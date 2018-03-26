#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"


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

  BeamTypeCmd = new G4UIcmdWithAString("/mu/gun/beamType",this);
  BeamTypeCmd->SetGuidance("Set beam type");
  BeamTypeCmd->SetParameterName("beamType",true);
  BeamTypeCmd->SetDefaultValue("storage");
  BeamTypeCmd->SetCandidates("storage injection");
  BeamTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  BeamSpinRotCmd = new G4UIcmdWithAString("/mu/gun/beamSpinRot",this);
  BeamSpinRotCmd->SetGuidance("Set beam spin rotation method");
  BeamSpinRotCmd->SetParameterName("beamSpinRot",true);
  BeamSpinRotCmd->SetDefaultValue("off");
  BeamSpinRotCmd->SetCandidates("off on");
  BeamSpinRotCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  BeamPolCmd = new G4UIcmdWithADouble("/mu/gun/beamPol",this);
  BeamPolCmd->SetGuidance("Set beam polarization magnitude");
  BeamPolCmd->SetParameterName("beamPol",true);
  BeamPolCmd->SetDefaultValue(0.5);
  BeamPolCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete BeamPolCmd;
  delete BeamSpinRotCmd;
  delete BeamTypeCmd;
  delete BeamSampleCmd;
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

  if( command == BeamTypeCmd )
    { Action->SetBeamType(newValue); }

  if( command == BeamSpinRotCmd )
    { Action->SetBeamSpinRotation(newValue); }

  if( command == BeamPolCmd )
    { Action->SetBeamPolarization(BeamPolCmd->GetNewDoubleValue(newValue)); }
}



