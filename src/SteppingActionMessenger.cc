#include "SteppingActionMessenger.hh"

#include "SteppingAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"


SteppingActionMessenger::SteppingActionMessenger(SteppingAction *stepAction):
  fStepAction(stepAction)
{
  fStepDir = new G4UIdirectory("/mu/step/");
  fStepDir->SetGuidance("SteppingAction control");

  fSaveStepCmd = new G4UIcmdWithABool("/mu/step/saveStep", this);
  fSaveStepCmd->SetGuidance("save stepping position or not");
  fSaveStepCmd->SetParameterName("SaveStep",false);
  fSaveStepCmd->SetDefaultValue(false);
  fSaveStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

SteppingActionMessenger::~SteppingActionMessenger()
{
  delete fSaveStepCmd;
  delete fStepDir;
}


void SteppingActionMessenger::SetNewValue(
					      G4UIcommand* command,G4String newValue)
{
  if( command == fSaveStepCmd )
    { fStepAction->SetSaveStep( fSaveStepCmd->GetNewBoolValue(newValue) );
    }
}
