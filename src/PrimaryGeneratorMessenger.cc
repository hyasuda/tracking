#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                          PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  gunDir = new G4UIdirectory("/mu/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/mu/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off repeat");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  BeamSampleCmd = new G4UIcmdWithAString("/mu/gun/beamSample",this);
  BeamSampleCmd->SetGuidance("Set beam sample file name");
  BeamSampleCmd->SetParameterName("beamSampleFile",true);
  BeamSampleCmd->SetDefaultValue("beamSample/sample_1200_good.txt");
  BeamSampleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete gunDir;
}


void PrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == RndmCmd )
   { Action->SetRndmFlag(newValue); }

  if( command == BeamSampleCmd )
    { Action->FillBeamSample(newValue); }
}



