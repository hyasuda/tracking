#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class SteppingActionMessenger;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  virtual ~SteppingAction();

  void UserSteppingAction(const G4Step*);
  void SetTimeStep(const G4double timeStep){fTimeStep=timeStep;}
  void SetSaveStep(const G4bool saveStep){fSaveStep=saveStep;}

private:
  SteppingActionMessenger* fMessenger;

  G4double fTimeStep;
  G4bool   fSaveStep;
};


#endif
