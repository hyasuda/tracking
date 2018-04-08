#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <vector>

class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(DetectorConstruction*);    
  virtual ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);
  void UpdateParticleDefinition();
  void SetRndmFlag(G4String val) { rndmFlag = val;}
  void FillBeamSample(G4String sampleFileName);
  void SetBeamType(G4String beamType){ fBeamType = beamType; }
  void SetBeamPolarization(G4double beamPol){ if(beamPol<0.){fBeamPol=0.;}else if(beamPol>1.){fBeamPol=1.;}else{fBeamPol = beamPol;} }
  void SetBeamSpinRotation(G4String beamSpinRot){fBeamSpinRot = beamSpinRot;}
  void SetStablePrimary(G4bool stablePrimary){ fStablePrimary = stablePrimary; }

private:
  G4ParticleGun*                particleGun;	  //pointer a to G4  class
  DetectorConstruction*    Detector;    //pointer to the geometry
    
  PrimaryGeneratorMessenger* gunMessenger;   //messenger of this class
  G4String                   rndmFlag;	  //flag for a rndm impact point
  G4String                   fBeamType;
  G4String                   fBeamSpinRot;
  G4double                   fBeamPol;
  G4bool                     fStablePrimary;
  G4int nEvent;

  std::vector<G4double>  fBeamX;
  std::vector<G4double>  fBeamY;
  std::vector<G4double>  fBeamZ;
  std::vector<G4double>  fBeamPx;
  std::vector<G4double>  fBeamPy;
  std::vector<G4double>  fBeamPz;
  std::vector<G4double>  fBeamP;
};


#endif


