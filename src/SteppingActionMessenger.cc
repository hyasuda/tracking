#include "SteppingActionMessenger.hh"
#include "SteppingAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"


SteppingActionMessenger::SteppingActionMessenger(SteppingAction *stepAction):
  fStepAction(stepAction)
{
  fStepDir = new G4UIdirectory("/mu/step/");
  fStepDir->SetGuidance("SteppingAction control");

  fTimeStepCmd = new G4UIcmdWithADoubleAndUnit("/mu/step/timeStep",this);
  fTimeStepCmd->SetGuidance("set stepping time interval");
  fTimeStepCmd->SetParameterName("TimeStep",true);
  fTimeStepCmd->SetUnitCategory("Time");
  fTimeStepCmd->SetDefaultValue(1.*ns);
  fTimeStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fSaveStepCmd = new G4UIcmdWithABool("/mu/step/saveStep", this);
  fSaveStepCmd->SetGuidance("save stepping position or not");
  fSaveStepCmd->SetParameterName("SaveStep",true);
  fSaveStepCmd->SetDefaultValue(false);
  fSaveStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

SteppingActionMessenger::~SteppingActionMessenger()
{
  delete fSaveStepCmd;
  delete fTimeStepCmd;
  delete fStepDir;
}


void SteppingActionMessenger::SetNewValue(
					      G4UIcommand* command,G4String newValue)
{
  if( command == fTimeStepCmd )
    { fStepAction->SetTimeStep( fTimeStepCmd->GetNewDoubleValue(newValue) );
    }
      
  if( command == fSaveStepCmd )
    { fStepAction->SetSaveStep( fSaveStepCmd->GetNewBoolValue(newValue) );
    }
}
