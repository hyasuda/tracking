#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;


class PrimaryGeneratorMessenger: public G4UImessenger
{
public:
  PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
  virtual ~PrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  PrimaryGeneratorAction*      Action;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithAString*          RndmCmd;
  G4UIcmdWithAString*          BeamSampleCmd;
  G4UIcmdWithAString*          BeamTypeCmd;
  G4UIcmdWithAString*          BeamSpinRotCmd;
  G4UIcmdWithADouble*          BeamPolCmd;
};


#endif

