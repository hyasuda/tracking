// $Id: ApplicationManager.hh,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#ifndef APPLICATIONMANAGER_HH
#define APPLICATIONMANAGER_HH

#include "G4ThreeVector.hh"
#include "globals.hh"
#include <fstream>

#include <vector>

#include <TROOT.h>
#include <TSystem.h>
#include <TApplication.h> 
#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TPad.h>
#include <TF1.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TRandom.h>
#include <TStopwatch.h>
#include <TGaxis.h>
#include <TLatex.h>
#include <TPaveStats.h>
#include <TError.h>

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "G4SystemOfUnits.hh"

class ApplicationManager
{
private:
  static ApplicationManager* theApplicationManager;
  
private:
  G4double thePrevTime;
  G4int theEventNum;
  G4int thePositronID;
  G4int theBeamIndex;
  std::ofstream theFileStream;

private:
  TFile* file;
  TApplication* theApplication;
  TTree* ntupleBody;
  TTree* ntupleDecay;
  TTree* ntupleTransport;
  
  std::vector<int> fPID;
  std::vector<int> bodyTyp;
  std::vector<int> bodyStatus;
  std::vector<double> EachDepE;
  std::vector<double> mom_x;
  std::vector<double> mom_y;
  std::vector<double> mom_z;
  std::vector<double> gtime;
  std::vector<double> pos_x;
  std::vector<double> pos_y;
  std::vector<double> pos_z;
  std::vector<double> tEnergy;
  std::vector<int> fIsPrimary;
  std::vector<int> fTrackID;

  std::vector<double> DkEnergy;
  std::vector<double> Dpol_x;
  std::vector<double> Dpol_y;
  std::vector<double> Dpol_z;
  std::vector<double> Dmom_x;
  std::vector<double> Dmom_y;
  std::vector<double> Dmom_z;
  std::vector<double> DtEnergy;
  std::vector<double> Dgtime;
  std::vector<double> Dpos_x;
  std::vector<double> Dpos_y;
  std::vector<double> Dpos_z;
  std::vector<int> DPDG;
  std::vector<int> DtrackID;
  std::vector<int> DparentID;

  std::vector<double> fTpos_x;
  std::vector<double> fTpos_y;
  std::vector<double> fTpos_z;
  std::vector<double> fTtEnergy;
  std::vector<double> fTmom_x;
  std::vector<double> fTmom_y;
  std::vector<double> fTmom_z;
  std::vector<double> fTtime;
  std::vector<double> fTpol_x;
  std::vector<double> fTpol_y;
  std::vector<double> fTpol_z;
  std::vector<double> fTmag_x;
  std::vector<double> fTmag_y;
  std::vector<double> fTmag_z;

public:
  ApplicationManager();
  ~ApplicationManager();

public:
  static ApplicationManager* GetApplicationManager();
  
public:
  G4double GetPrevTime() const {return thePrevTime;}
  G4int GetEventNum() const {return theEventNum;}
  G4int GetPositronID() const {return thePositronID;}
  G4int GetBeamIndex() const { return theBeamIndex; }
  std::ofstream& GetFileStream(){return theFileStream;}

  void SetPrevTime(G4double time){thePrevTime=time;}
  void SetEventNum(G4int eventNum){theEventNum=eventNum;}
  void SetPositronID(G4int trackID){thePositronID=trackID;}
  void SetBeamIndex(G4int beamIndex){ theBeamIndex = beamIndex; }

  TApplication* GetApplication() const {return theApplication;}
  void Clear(){gSystem->ProcessEvents();}
  void Update(){gSystem->ProcessEvents();}
  void Open(G4String fileNameString);
  void Save();

  void FillNtuple(){ntupleBody->Fill();} 
  void FillDecayNtuple(){ntupleDecay->Fill();} 
  void FillTransportNtuple(){ntupleTransport->Fill();}
  void PutNtupleValue(G4int pID, G4double tE, G4ThreeVector pos, G4ThreeVector mom, G4double Gtime, G4int bodyTyp, G4int bodyStatus, G4double EachDepE, G4int isPrimary, G4int trackID);
  void PutDecayValue(G4double DTEnergy, G4ThreeVector Dpos, G4ThreeVector Dmom, G4ThreeVector Dpol,G4double DGtime, G4int PDG, G4int trackID, G4int parentID);
  void PutTransportValue(G4double TtEnergy, G4ThreeVector Tmom, G4double Ttime, G4ThreeVector Tpos, G4ThreeVector Tpol, G4ThreeVector Tmag);
  void ClearNtuple(); 
    
};

inline ApplicationManager* ApplicationManager::GetApplicationManager()
{
  if ( theApplicationManager == 0 ) {
    theApplicationManager = new ApplicationManager();
  }
  return theApplicationManager;
}

#endif
