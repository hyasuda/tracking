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
  void SetRndmFlag(G4String val) { rndmFlag = val;}

private:
  G4ParticleGun*                particleGun;	  //pointer a to G4  class
  DetectorConstruction*    Detector;    //pointer to the geometry
    
  PrimaryGeneratorMessenger* gunMessenger;   //messenger of this class
  G4String                      rndmFlag;	  //flag for a rndm impact point

  std::vector<G4double>  fBeamX;
  std::vector<G4double>  fBeamY;
  std::vector<G4double>  fBeamZ;
  std::vector<G4double>  fBeamPx;
  std::vector<G4double>  fBeamPy;
  std::vector<G4double>  fBeamPz;
};


#endif


