#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class EventActionMessenger;


class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*);
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
    
  void AddAbs(G4double de, G4double dl) {EnergyAbs += de; TrackLAbs += dl;};
                     
  void SetPrintModulo(G4int    val)  {printModulo = val;};
    
private:
   RunAction*  runAct;
   
   G4double  EnergyAbs;
   G4double  TrackLAbs;
                     
   G4int     printModulo;
                             
   EventActionMessenger*  eventMessenger;

};


#endif

    
