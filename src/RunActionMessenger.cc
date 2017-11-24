#include "RunActionMessenger.hh"
#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


RunActionMessenger::RunActionMessenger(
				       RunAction* runaction)
  :run(runaction)
{
  runDir = new G4UIdirectory("/mu/run/");
  runDir->SetGuidance("RunAction control");

  fileNameCmd = new G4UIcmdWithAString("/mu/run/fileName",this);
  fileNameCmd->SetGuidance("set output file name if necessary");
  fileNameCmd->SetGuidance("otherwise file name is named automatically");
  fileNameCmd->SetParameterName("fileName",true);
  fileNameCmd->SetDefaultValue("");
  fileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


RunActionMessenger::~RunActionMessenger()
{
  delete fileNameCmd;
  delete runDir;
}


void RunActionMessenger::SetNewValue(
				     G4UIcommand* command, G4String newValue)
{
  if( command == fileNameCmd )
    { run->SetFileName(newValue); }
}
