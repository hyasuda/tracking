#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonPlus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "Randomize.hh"
#include "TMath.h"

#include "G4SystemOfUnits.hh"

#include <fstream>

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
    = particleTable->FindParticle(particleName="mu+");

  particleGun->SetParticleDefinition(particle);

  std::ifstream ifs("beamSample/sample_1200_good.txt");
  char buf[1024];
  G4int count = 0;
  G4double val[8];
  while(!ifs.eof()){
    ifs.getline(buf,1024);
    if(ifs.eof()) break;
    if(count>0){
      strtok(buf,","); // id
      for(unsigned int i=0; i<8; i++){
	val[i] = strtod(strtok(NULL,","),NULL);
      }
      fBeamX.push_back(val[1]);
      fBeamY.push_back(val[3]);
      fBeamZ.push_back(val[2]);
      fBeamPx.push_back(-val[4]);
      fBeamPy.push_back(-val[6]);
      fBeamPz.push_back(val[5]);
    }
    count++;
  }
  ifs.close();

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

  G4double x0 = -0.333*m;//hiromiPOSITION
  G4double y0 = 0*m; 
  G4double z0 = 0*m;

  G4double px0 = 0.;
  G4double py0 = 1.;
  G4double pz0 = 0.;

  G4double polx0 = 0.;
  G4double poly0 = 1.;
  G4double polz0 = 0.;

  if(rndmFlag=="on"){
    G4double rand = G4UniformRand();
    G4int randI = (G4int)(fBeamX.size()*rand);
    x0 = fBeamX.at(randI)*m;
    y0 = fBeamY.at(randI)*m;
    z0 = fBeamZ.at(randI)*m;

    px0 = fBeamPx.at(randI);
    py0 = fBeamPy.at(randI);
    pz0 = fBeamPz.at(randI);

    polx0 = px0;
    poly0 = py0;
    polz0 = pz0;

    // polarization magnitude
    const G4double polMag = 0.5;
    G4double rand2 = G4UniformRand();
    if(rand2<0.5*(1-polMag)){
      polx0 = -polx0;
      poly0 = -poly0;
      polz0 = -polz0;
    }

    // spin precession
    const G4double omega = 2.9752/microsecond; 
    const G4double spentTime = 220.*ns;
    const G4double phase = spentTime*omega;
    
    G4double polx0_temp = polx0;
    G4double poly0_temp = poly0;
    polx0 = polx0_temp*cos(phase)-poly0_temp*sin(phase);
    poly0 = -polx0_temp*sin(phase)+poly0_temp*cos(phase);
  }

  particleGun->SetParticleEnergy(Kmu*MeV);//Kinetic energy

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));//Initial position
  particleGun->SetParticlePolarization(G4ThreeVector(polx0,poly0,polz0));//Initial polarization
  particleGun->SetParticleMomentumDirection(G4ThreeVector(px0,py0,pz0));//Initial momentum direction
  
  particleGun->GeneratePrimaryVertex(anEvent);
}



