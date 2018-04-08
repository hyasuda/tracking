#ifndef BODYSD_HH
#define BODYSD_HH

#include <unordered_set>
 
#include "G4VSensitiveDetector.hh"
#include "RunAction.hh" 
#include "CalHit.hh"

class G4HCofThisEbent;
class G4Step;
class G4TouchableHistory;

enum { NCHANNEL=2000};

class BodySD
  : public G4VSensitiveDetector
{

public:
  BodySD();
  ~BodySD();
  
  public:
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  
  virtual void Initialize(G4HCofThisEvent* HCTE);
  virtual void EndOfEvent(G4HCofThisEvent* HCTE);
  virtual void DrawAll();
  virtual void PrintAll();

private:
  CalHitsCollection* hitsCollection;
  G4double edepbuf[NCHANNEL];
  G4int currentTrackID;
  G4double currentTotalDepE;
  G4double unsummedDepE;
  G4double steplengthTotal;

  std::unordered_set<G4int> fHitTrackID;

};

#endif
