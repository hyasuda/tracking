#ifndef BODYSD_HH
#define BODYSD_HH
 
#include "G4VSensitiveDetector.hh"
#include "RunAction.hh" 
#include "CalHit.hh"

//class TTree;
//class TFile;
//class TH1D;

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
//     virtual void clear();
     virtual void DrawAll();
     virtual void PrintAll();
//     virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) = 0;

  private:
     CalHitsCollection* hitsCollection;
     G4double edepbuf[NCHANNEL];
};

#endif
