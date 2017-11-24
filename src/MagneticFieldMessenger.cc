#include "MagneticFieldMessenger.hh"

#include "MagneticField.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"


MagneticFieldMessenger::MagneticFieldMessenger(MagneticField* mag)
:fMag(mag)
{ 
  fMagDir = new G4UIdirectory("/mu/mag/");
  fMagDir->SetGuidance("MagneticField control");

  fCalTypeCmd = new G4UIcmdWithAString("/mu/mag/calType", this);
  fCalTypeCmd->SetGuidance("choose MagneticField calculation type");
  fCalTypeCmd->SetParameterName("CalType",true);
  fCalTypeCmd->SetDefaultValue("uniform");
  fCalTypeCmd->SetCandidates("uniform interpolation interpolationstorage strict");
  fCalTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fFieldFileCmd = new G4UIcmdWithAString("/mu/mag/fieldFile", this);
  fFieldFileCmd->SetGuidance("Set magnetic field calculation file");
  fFieldFileCmd->SetParameterName("fieldFile",true);
  //fFieldFileCmd->SetDefaultValue("FLDATA/20160422_Abe2017May-77_n15.txt");
  fFieldFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fWithSpinCmd = new G4UIcmdWithABool("/mu/mag/withSpin", this);
  fWithSpinCmd->SetGuidance("choose MagneticField track spin");
  fWithSpinCmd->SetParameterName("WithSpin",true);
  fWithSpinCmd->SetDefaultValue(false);
  fWithSpinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}



MagneticFieldMessenger::~MagneticFieldMessenger()
{
  delete fCalTypeCmd;
  delete fWithSpinCmd;
  delete fMagDir;
}


void MagneticFieldMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fCalTypeCmd )
    { fMag->SetCalType(newValue);
      /*fMag->FillFieldValue();*/ }

  if( command == fFieldFileCmd )
    { fMag->SetFieldFileName(newValue);
      fMag->FillFieldValue(); }

  if( command == fWithSpinCmd )
    { fMag->SetWithSpin(newValue);
      fMag->updateField(); }
}


