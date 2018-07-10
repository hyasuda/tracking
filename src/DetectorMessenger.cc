#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
//#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
:Detector(Det)
{ 
  muDir = new G4UIdirectory("/mu/");
  muDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/mu/det/");
  detDir->SetGuidance("detector control");

  UpdateCmd = new G4UIcmdWithoutParameter("/mu/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  NbVanesCmd = new G4UIcmdWithAnInteger("/mu/det/nvanes",this);
  NbVanesCmd->SetGuidance("set the number of vanes of detector");
  NbVanesCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDoStripCmd = new G4UIcmdWithABool("/mu/det/doStrip",this);
  fDoStripCmd->SetGuidance("segment sensor into strip");
  fDoStripCmd->SetParameterName("DoStrip",true);
  fDoStripCmd->SetDefaultValue(true);
  fDoStripCmd->AvailableForStates(G4State_Idle);
  
}



DetectorMessenger::~DetectorMessenger()
{
  delete fDoStripCmd;
  delete UpdateCmd;
  delete NbVanesCmd;
  delete detDir;
  delete muDir;  
}


void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

  if( command == NbVanesCmd )
    { if(Detector->SetNumberOfVanes(NbVanesCmd->GetNewIntValue(newValue))){
	Detector->UpdateGeometry();
      }
    }

  if( command == fDoStripCmd )
    { if(Detector->SetDoStrip(fDoStripCmd->GetNewBoolValue(newValue))){
	Detector->UpdateGeometry();
      }
    }
}


