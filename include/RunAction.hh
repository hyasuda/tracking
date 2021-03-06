#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double); 
  void SetFileName(G4String fileName){ theFileName = fileName; }

private:
  RunActionMessenger* runmessenger;

  G4double sumEAbs, sum2EAbs;    
  G4double sumLAbs, sum2LAbs;

  G4String theFileName;

  //DataBroker 
  G4String AsString( G4int runid, G4int digit = 6 ) const;

};


#endif

