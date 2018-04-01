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
  G4double theEdepByEvent;
  G4double theEdepByRun;
  G4ThreeVector theHitPosition;
  G4ThreeVector theHitMomentum;
  G4double theHitTEnergy;
  G4double theHitKEnergy;
  G4double theHitGtime;
  G4double theHitPtime;
  G4double thePrevTime;
  G4int theParID;
  G4int theHitBodyTyp;
  G4int theHitInfo;
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
  
  int   fEventNum;
  int   fHitInfo;
  
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
    G4double GetEdepByEvent() const;
    G4double GetEdepByRun() const;
    const G4ThreeVector& GetHitPosition() const;
    const G4ThreeVector& GetHitMomentum() const;
    G4double GetHitKEnergy() const;
    G4double GetHitTEnergy() const;
    G4double GetHitGtime() const;
    G4double GetHitPtime() const;
    G4double GetPrevTime() const;
    G4int GetEventNum() const;
    G4int GetHitInfo() const;
    G4int GetHitBody() const;
    G4int GetParID() const;
    G4int GetPositronID() const;
    G4int GetBeamIndex() const { return theBeamIndex; }
    std::ofstream& GetFileStream();
    void SetEdepByEvent(G4double edep);
    void SetEdepByRun(G4double edep);
    void SetHitPosition(const G4ThreeVector& pos);
    void SetHitMomentum(const G4ThreeVector& mom);
    void SetHitTEnergy(G4double Tenergy);
    void SetHitKEnergy(G4double Kenergy);
    void SetPrevTime(G4double time);
    void SetEventNum(G4int eventNum);
    void SetHitInfo(G4int hitInfo);
    void SetHitGtime(G4double Gtime);
    void SetHitPtime(G4double Ptime);
    void SetHitBody(G4int theHitBodyTyp);
    void SetParID(G4int theParID);
    void SetPositronID(G4int trackID);
    void SetBeamIndex(G4int beamIndex){ theBeamIndex = beamIndex; }

    void AddEdepByEvent(G4double edep);
    void AddEdepByRun(G4double edep);

    TApplication* GetApplication() const;
    TTree* GetNtupleBody() const;
    void Clear();
    void Update();
    void Open(G4String fileNameString);
    void Save();
    void Fill(G4double edep );
    void Fill(G4double x, G4double y, G4double edep);
    void FillNtuple(); 
    void FillDecayNtuple(); 
    void PutNtupleValue(G4int pID, G4double tE, G4ThreeVector pos, G4ThreeVector mom, G4double Gtime, G4int bodyTyp, G4int bodyStatus, G4int hitInfo, G4double EachDepE, G4int isPrimary, G4int trackID);
  void PutDecayValue(G4double DTEnergy, G4ThreeVector Dpos, G4ThreeVector Dmom, G4ThreeVector Dpol,G4double DGtime, G4int PDG, G4int trackID, G4int parentID);
     void ClearNtuple(); 
    void FillTransportNtuple(); 
    void PutTransportValue(G4int evtNum, G4double TtEnergy, G4ThreeVector Tmom, G4double Ttime, G4ThreeVector Tpos, G4ThreeVector Tpol, G4ThreeVector Tmag);

};

inline ApplicationManager* ApplicationManager::GetApplicationManager()
{
  if ( theApplicationManager == 0 ) {
    theApplicationManager = new ApplicationManager();
  }
  return theApplicationManager;
}

inline std::ofstream& ApplicationManager::GetFileStream()
{
  return theFileStream;
}

inline G4double ApplicationManager::GetEdepByEvent() const
{
  return theEdepByEvent;
}

inline G4double ApplicationManager::GetEdepByRun() const
{
  return theEdepByRun;
}

inline const G4ThreeVector& ApplicationManager::GetHitPosition() const
{
  return theHitPosition;
}

inline const G4ThreeVector& ApplicationManager::GetHitMomentum() const
{
  return theHitMomentum;
}

inline G4double ApplicationManager::GetHitKEnergy() const
{
  return theHitKEnergy;
}

inline G4int ApplicationManager::GetHitInfo() const
{
  return theHitInfo;
}

inline G4int ApplicationManager::GetEventNum() const
{
  return theEventNum;
}

inline G4double ApplicationManager::GetHitTEnergy() const
{
  return theHitTEnergy;
}

inline G4double ApplicationManager::GetHitGtime() const
{
  return theHitGtime;
}

inline G4double ApplicationManager::GetHitPtime() const
{
  return theHitPtime;
}

inline G4double ApplicationManager::GetPrevTime() const
{
  return thePrevTime;
}

inline G4int ApplicationManager::GetHitBody() const
{
  return theHitBodyTyp;
}
inline G4int ApplicationManager::GetParID() const
{
  return theParID;
}
inline G4int ApplicationManager::GetPositronID() const
{
  return thePositronID;
}

inline void ApplicationManager::SetEdepByEvent(G4double edep)
{
  theEdepByEvent = edep;
  return;
}

inline void ApplicationManager::SetEdepByRun(G4double edep)
{
  theEdepByRun = edep;
  return;
}

inline void ApplicationManager::SetHitPosition(const G4ThreeVector& pos)
{
  theHitPosition = pos;
  return;
}

inline void ApplicationManager::SetHitMomentum(const G4ThreeVector& mom)
{
  theHitMomentum = mom;
  return;
}

inline void ApplicationManager::SetHitKEnergy(G4double Kenergy)
{
  theHitKEnergy = Kenergy;
  return;
}

inline void ApplicationManager::SetHitTEnergy(G4double Tenergy)
{
  theHitTEnergy = Tenergy;
  return;
}

inline void ApplicationManager::SetHitInfo(G4int hitInfo) 
{
  theHitInfo=hitInfo;
  return;
}

inline void ApplicationManager::SetEventNum(G4int eventNum)
{
  theEventNum=eventNum;
  return;
}

inline void ApplicationManager::SetHitGtime(G4double Gtime)
{
  theHitGtime = Gtime;
  return;
}

inline void ApplicationManager::SetHitPtime(G4double Ptime)
{
  theHitPtime = Ptime;
  return;
}

inline void ApplicationManager::SetPrevTime(G4double time)
{
  thePrevTime = time;
  return;
}

inline void ApplicationManager::SetHitBody(G4int Body)
{
  theHitBodyTyp = Body;
  return;
}
inline void ApplicationManager::SetParID(G4int pID)
{
  theParID = pID;
  return;
}
inline void ApplicationManager::SetPositronID(G4int trackID)
{
  thePositronID = trackID;
  return;
}

inline void ApplicationManager::AddEdepByEvent(G4double edep)
{
  theEdepByEvent += edep;
  return;
}

inline void ApplicationManager::AddEdepByRun(G4double edep)
{
  theEdepByRun += edep;
  return;
}

inline TApplication* ApplicationManager::GetApplication() const
{
  return theApplication;
}

inline TTree* ApplicationManager::GetNtupleBody() const
{
  return ntupleBody;
}

inline void ApplicationManager::Update()
{
  gSystem->ProcessEvents();
  return;
}

inline void ApplicationManager::Clear()
{
  gSystem->ProcessEvents();
  return;
}

inline void ApplicationManager::Open(G4String fileNameString)
{
  char filename[256];
  if(fileNameString==""){
    // 170113tyosioka
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    char buff[128]="";
    sprintf(buff,"%04d%02d%02d%02d%02d%02d",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
    
    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    char del[] = ".";
    char *tok;
    tok = strtok(hostname,del);

    sprintf(filename,"data/mug2edm_%s_%s.root",buff,tok);
  }else{
    sprintf(filename,"%s",fileNameString.c_str());
  }
  file= new TFile( filename, "RECREATE", "Geant4 User Application" );
  return;
}

inline void ApplicationManager::Save()
{
  file->cd();
  printf("writen ROOT File\n"); 
  ntupleBody->Write();
  ntupleDecay->Write();
  ntupleTransport->Write();
  file->Close();
  delete file;
  return;
}

inline void ApplicationManager::ClearNtuple() 
{
  Dpol_x.clear();
  Dpol_y.clear();
  Dpol_z.clear();
  Dmom_x.clear();
  Dmom_y.clear();
  Dmom_z.clear();
  DtEnergy.clear();
  Dpos_x.clear();
  Dpos_y.clear();
  Dpos_z.clear();
  Dgtime.clear();
  DPDG.clear();
  DtrackID.clear();
  DparentID.clear();

  EachDepE.clear();
  fPID.clear();
  bodyTyp.clear();
  bodyStatus.clear();
  mom_x.clear();
  mom_y.clear();
  mom_z.clear();
  gtime.clear();
  pos_x.clear();
  pos_y.clear();
  pos_z.clear();
  tEnergy.clear();
  fIsPrimary.clear();
  fTrackID.clear();

  fTmom_x.clear();
  fTmom_y.clear();
  fTmom_z.clear();
  fTtEnergy.clear();
  fTpos_x.clear();
  fTpos_y.clear();
  fTpos_z.clear();
  fTtime.clear();
  fTpol_x.clear();
  fTpol_y.clear();
  fTpol_z.clear();
  fTmag_x.clear();
  fTmag_y.clear();
  fTmag_z.clear();

  fHitInfo=0;
  theHitInfo=0;
  //thePrevTime=0.;
  thePrevTime=-1e9*second;

  return;
}


inline void ApplicationManager::PutDecayValue(G4double DTEnergy, G4ThreeVector Dpos, G4ThreeVector Dmom, G4ThreeVector Dpol,G4double DGtime, G4int PDG, G4int trackID, G4int parentID)
{
  DtEnergy.push_back(DTEnergy);
  Dpos_x.push_back(Dpos.x());
  Dpos_y.push_back(Dpos.y());
  Dpos_z.push_back(Dpos.z());
  Dmom_x.push_back(Dmom.x());
  Dmom_y.push_back(Dmom.y());
  Dmom_z.push_back(Dmom.z());
  Dpol_x.push_back(Dpol.x());
  Dpol_y.push_back(Dpol.y());
  Dpol_z.push_back(Dpol.z());
  Dgtime.push_back(DGtime);
  DPDG.push_back(PDG);
  DtrackID.push_back(trackID);
  DparentID.push_back(parentID);
}
inline void ApplicationManager::PutNtupleValue(G4int parID, G4double TEnergy, G4ThreeVector pos, G4ThreeVector mom, G4double Gtime, G4int bodyType, G4int bodyStat, G4int hitInformation,G4double eachDepE,G4int isPrimary, G4int trackID)
{
  fHitInfo=hitInformation;

  EachDepE.push_back(eachDepE/MeV);
  bodyTyp.push_back(bodyType);
  bodyStatus.push_back(bodyStat);
  fPID.push_back(parID);
  mom_x.push_back(mom.x());
  mom_y.push_back(mom.y());
  mom_z.push_back(mom.z());
  gtime.push_back(Gtime);//nsec
  pos_x.push_back(pos.x());
  pos_y.push_back(pos.y());
  pos_z.push_back(pos.z());
  tEnergy.push_back(TEnergy/MeV);
  fIsPrimary.push_back(isPrimary);
  fTrackID.push_back(trackID);

  return;

}

inline void ApplicationManager::PutTransportValue(G4int eventNum, G4double TtEnergy, G4ThreeVector Tmom, G4double Ttime, G4ThreeVector Tpos, G4ThreeVector Tpol, G4ThreeVector Tmag)
{
  fEventNum = eventNum;
  fTtEnergy.push_back(TtEnergy/MeV);
  fTmom_x.push_back(Tmom.x());
  fTmom_y.push_back(Tmom.y());
  fTmom_z.push_back(Tmom.z());
  fTtime.push_back(Ttime);
  fTpos_x.push_back(Tpos.x());
  fTpos_y.push_back(Tpos.y());
  fTpos_z.push_back(Tpos.z());
  fTpol_x.push_back(Tpol.x());
  fTpol_y.push_back(Tpol.y());
  fTpol_z.push_back(Tpol.z());
  fTmag_x.push_back(Tmag.x());
  fTmag_y.push_back(Tmag.y());
  fTmag_z.push_back(Tmag.z());
  return;
}

inline void ApplicationManager::FillNtuple()
{
  ntupleBody->Fill();
  return;
}

inline void ApplicationManager::FillDecayNtuple()
{
  ntupleDecay->Fill();
  return;
}

inline void ApplicationManager::FillTransportNtuple()
{
  ntupleTransport->Fill();
  return;
}

#endif
