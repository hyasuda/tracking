#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonPlus.hh"
#include "G4Electron.hh"
#include "Randomize.hh"
#include "TMath.h"

#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(
                                             DetectorConstruction* DC)
:Detector(DC),rndmFlag("off")
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="mu+");//hiromi

  particleGun->SetParticleDefinition(particle);
  //particleGun->SetParticlePolarization(G4ThreeVector(1.,0.,0.));//hiromi

}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 
  double mu_mass = G4MuonPlus::MuonPlus()->GetPDGMass()*MeV;
  double Pmu = 300*MeV; // /c
  double Emu = sqrt(Pmu*Pmu+mu_mass*mu_mass);
  double Kmu = Emu-mu_mass;

  /*
  double rnd=rand()%1000;
  rnd=rnd/(double)1000;

  double rnd2=rand()%2000;
  rnd2=rnd2/(double)2000;
  */

  G4double x0 = -0.333*m;//hiromiPOSITION
  G4double y0 = 0*m; 
  G4double z0 = 0*m;

  particleGun->SetParticleEnergy(Kmu*MeV);//Kinetic energy

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));//Initial position
  particleGun->SetParticlePolarization(G4ThreeVector(0.,1.,0.));//Initial polarization
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));//Initial momentum direction

  particleGun->GeneratePrimaryVertex(anEvent);
}



