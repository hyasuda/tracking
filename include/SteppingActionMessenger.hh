#ifndef SteppingActionMessenger_h
#define SteppingActionMessenger_h

#include "globals.hh"
#include "G4UImessenger.hh"

class SteppingAction;
class G4UIdirectory;
class G4UIcmdWithABool;

class SteppingActionMessenger: public G4UImessenger
{
public:
  SteppingActionMessenger(SteppingAction *);
  ~SteppingActionMessenger();

  void SetNewValue(G4UIcommand*,G4String newValue);

private:
  SteppingAction   *fStepAction;
  G4UIdirectory    *fStepDir;
  G4UIcmdWithABool *fSaveStepCmd;
};

#endif
