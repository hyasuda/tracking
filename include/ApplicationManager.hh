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
    G4int theParID;
    G4int theHitBodyTyp;
    G4int theHitInfo;
    G4int theEventNum;
    G4int thePositronID;
    std::ofstream theFileStream;

  private:
    TFile* file;
    TApplication* theApplication;
    TH1D* theEdepHist;
    TH2D* theHitHist;
    TTree* ntupleBody;
    TTree* ntupleDecay;

    int   fEventNum;
    int   fHitInfo;

    std::vector<int> fPID;
    std::vector<int> bodyTyp;
    std::vector<int> bodyStatus;
    std::vector<int> chID;//not yet hiromi 2010/05/25
    std::vector<double> EachDepE;
    std::vector<double> CurrentDepE;
    std::vector<double> kEnergy;
    std::vector<double> mom_x;
    std::vector<double> mom_y;
    std::vector<double> mom_z;
    std::vector<double> ptime;
    std::vector<double> gtime;
    std::vector<double> pos_x;
    std::vector<double> pos_y;
    std::vector<double> pos_z;
    std::vector<double> tEnergy;
    std::vector<int> fIsPrimary;
    std::vector<int> fTrackID;
  //std::vector<double> fStepLength;
  //std::vector<double> fStepLengthTotal;

    float DkEnergy[4];
    float Dpol_x;
    float Dpol_y;
    float Dpol_z;
    float Dmomv_x[4];
    float Dmomv_y[4];
    float Dmomv_z[4];
    float Dmom_x[4];
    float Dmom_y[4];
    float Dmom_z[4];
    float Dptime;
    float Dgtime;
    float Dpos_x;
    float Dpos_y;
    float Dpos_z;
    float DtEnergy[4];
  //    float TotDep;
    int   DPDG[4];

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
    G4int GetEventNum() const;
    G4int GetHitInfo() const;
    G4int GetHitBody() const;
    G4int GetParID() const;
    G4int GetPositronID() const;
    std::ofstream& GetFileStream();
    void SetEdepByEvent(G4double edep);
    void SetEdepByRun(G4double edep);
    void SetHitPosition(const G4ThreeVector& pos);
    void SetHitMomentum(const G4ThreeVector& mom);
    void SetHitTEnergy(G4double Tenergy);
    void SetHitKEnergy(G4double Kenergy);
    void SetEventNum(G4int eventNum);
    void SetHitInfo(G4int hitInfo);
    void SetHitGtime(G4double Gtime);
    void SetHitPtime(G4double Ptime);
    void SetHitBody(G4int theHitBodyTyp);
    void SetParID(G4int theParID);
    void SetPositronID(G4int trackID);

    void AddEdepByEvent(G4double edep);
    void AddEdepByRun(G4double edep);

    TApplication* GetApplication() const;
    TH1D* GetEdepHist() const;
    TH2D* GetHitHist() const;
    TTree* GetNtupleBody() const;
    void Clear();
    void Update();
    void Open(G4String fileNameString);
    void Save();
    void Fill(G4double edep );
    void Fill(G4double x, G4double y, G4double edep);
    void FillNtuple(); 
    void FillDecayNtuple(); 
    void PutNtupleValue(G4int pID,G4double kE, G4double tE, G4ThreeVector pos, G4ThreeVector mom, 
			G4double Gtime, G4double Ptime, G4int bodyTyp, G4int bodyStatus, G4int chID, G4int evtNum, G4int hitInfo, G4double CurrentDepE, G4double EachDepE, G4int isPrimary, G4int trackID/*, G4double stepLength, G4double stepLengthTotal*/);
    void PutDecayValue(G4int evtNum,G4double DKEnergy, G4double DTEnergy, G4ThreeVector Dpos, G4ThreeVector Dmom, 
              G4ThreeVector Dmomv, G4ThreeVector Dpol,G4double DGtime, G4double DPtime, G4int passID);
    void ClearNtuple(G4int eventNum); 

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

inline TH1D* ApplicationManager::GetEdepHist() const
{
  return theEdepHist;
}

inline TH2D* ApplicationManager::GetHitHist() const
{
  return theHitHist;
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
  theEdepHist->Reset();
  theHitHist->Reset();
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
  //theEdepHist->Write();
  //theHitHist->Write();
  ntupleBody->Write();
  ntupleDecay->Write();
  file->Close();
  delete file;
  return;
}


inline void ApplicationManager::Fill(G4double edep)
{
  theEdepHist->Fill(edep);
  return;
}

inline void ApplicationManager::Fill(G4double x, G4double y, G4double edep)
{
  theHitHist->Fill(x, y, edep);
  return;
}

inline void ApplicationManager::ClearNtuple(G4int evtNum) 
{

  fEventNum= evtNum;
  for(int i=0;i<4;++i){
    DkEnergy[i] = 0;
    Dmomv_x[i] = 0;
    Dmomv_y[i] = 0;
    Dmomv_z[i] = 0;
    Dmom_x[i] = 0;
    Dmom_y[i] = 0;
    Dmom_z[i] = 0;
    DtEnergy[i]= 0;
    DPDG[i] = 0;
 }

  //kEnergy.clear();
  EachDepE.clear();
  CurrentDepE.clear();
  fPID.clear();
  bodyTyp.clear();
  bodyStatus.clear();
  //chID.clear();
  mom_x.clear();
  mom_y.clear();
  mom_z.clear();
  //ptime.clear();//nsec
  gtime.clear();//nsec
  pos_x.clear();
  pos_y.clear();
  pos_z.clear();
  tEnergy.clear();
  fIsPrimary.clear();
  fTrackID.clear();
  //fStepLength.clear();
  //fStepLengthTotal.clear();

  fHitInfo=0;
  theHitInfo=0;

  return;
}


inline void ApplicationManager::PutDecayValue(G4int evtNum,G4double DKEnergy, G4double DTEnergy, 
G4ThreeVector Dpos, G4ThreeVector Dmom, G4ThreeVector Dmomv, G4ThreeVector Dpol,G4double DGtime, G4double DPtime, G4int passID)
{
  if(passID<0 || passID>3) return;

  if(passID==0){//mu+
    fEventNum= evtNum;
    Dpol_x = Dpol.x();
    Dpol_y = Dpol.y();
    Dpol_z = Dpol.z();
    Dptime = DPtime;//nsec
    Dgtime = DGtime;//nsec
    Dpos_x = Dpos.x();
    Dpos_y = Dpos.y();
    Dpos_z = Dpos.z();
  
  }
  DtEnergy[passID]= DTEnergy/MeV;
  DkEnergy[passID]= DKEnergy/MeV;
  Dmom_x[passID] = Dmom.x();
  Dmom_y[passID] = Dmom.y();
  Dmom_z[passID] = Dmom.z();
  Dmomv_x[passID] = Dmomv.x();
  Dmomv_y[passID] = Dmomv.y();
  Dmomv_z[passID] = Dmomv.z();
  int PDG = 0;
  if(passID==0) PDG = -13;
  else if(passID==1) PDG = -11;
  else if(passID==2) PDG = 12;
  else if(passID==3) PDG = -14;
  DPDG[passID] = PDG;

  return;

}
inline void ApplicationManager::PutNtupleValue(G4int parID, G4double KEnergy, G4double TEnergy, 
					       G4ThreeVector pos, G4ThreeVector mom, G4double Gtime, G4double Ptime, G4int bodyType, G4int bodyStat, G4int chNum, G4int evtNum, G4int hitInformation,G4double DepEByEve,G4double eachDepE,G4int isPrimary, G4int trackID)
{
 
  fHitInfo=hitInformation;
  fEventNum= evtNum;

  CurrentDepE.push_back(DepEByEve/MeV);
  EachDepE.push_back(eachDepE/MeV);
  kEnergy.push_back(KEnergy/MeV);
  bodyTyp.push_back(bodyType);
  bodyStatus.push_back(bodyStat);
  chID.push_back(chNum);
  fPID.push_back(parID);
  mom_x.push_back(mom.x());
  mom_y.push_back(mom.y());
  mom_z.push_back(mom.z());
  ptime.push_back(Ptime);//nsec
  gtime.push_back(Gtime);//nsec
  pos_x.push_back(pos.x());
  pos_y.push_back(pos.y());
  pos_z.push_back(pos.z());
  tEnergy.push_back(TEnergy/MeV);
  fIsPrimary.push_back(isPrimary);
  fTrackID.push_back(trackID);

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
#endif
