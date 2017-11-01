#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


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
   
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/mu/det/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
 
  NbVanesCmd = new G4UIcmdWithAnInteger("/mu/det/nvanes",this);
  NbVanesCmd->SetGuidance("set the number of vanes of detector");
  NbVanesCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}



DetectorMessenger::~DetectorMessenger()
{
  delete UpdateCmd;
  delete MagFieldCmd;
  delete NbVanesCmd;
  delete detDir;
  delete muDir;  
}


void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

  if( command == MagFieldCmd )
    { Detector->SetMagField(/*MagFieldCmd->GetNewDoubleValue(newValue)*/);}

  if( command == NbVanesCmd )
    { Detector->SetNumberOfVanes(NbVanesCmd->GetNewIntValue(newValue));
      Detector->UpdateGeometry();}
}


