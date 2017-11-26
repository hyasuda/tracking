#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class DetectorConstruction;
class EventAction;
class TTree;
class TFile;
class TH1D;


class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(DetectorConstruction*, EventAction*);
  virtual ~SteppingAction();

  void UserSteppingAction(const G4Step*);
    
private:
  DetectorConstruction* detector;
  EventAction*          eventaction;  

  TTree *ntuple;
  TFile *Geantfile;
  TH1D* hist_wiggle;

  G4double fTimeStep;

  int eventNum;
  float primEnergy[4];//0=>mu+, 1=>e+, 2=>e-, 3=>gamma
  float pol_x,pol_y,pol_z;
  float momv_x[4],momv_y[4],momv_z[4];
  float mom_x[4],mom_y[4],mom_z[4];
  float ptime;//nsec
  float gtime;//nsec
  float pos_x,pos_y,pos_z;
  float tot_energy[4];
  float TotalDep;

};


#endif
