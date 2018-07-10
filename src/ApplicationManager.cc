// $Id: ApplicationManager.cc,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#include <ApplicationManager.hh>
#include "G4ExceptionHandler.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TFile.h" 
#include "TTree.h" 

#include <vector>

ApplicationManager* ApplicationManager::theApplicationManager = 0;

ApplicationManager::ApplicationManager()
  : theFileStream(),
    theApplication(0)
{
  if ( theApplicationManager ) {
    G4cout <<  "ApplicationManager constructed twice." << G4endl;
    exit(0);
  }
  theApplicationManager = this;

  theApplication = new TApplication("Geant4 User Application", 0, 0);

  //thePrevTime = 0.;
  thePrevTime = -1e9*second;
  theBeamIndex = -1;

  gErrorIgnoreLevel = 3000;

  ntupleBody= new TTree ("ntupleBody","Tree for MCSteps and SimHits");
  ntupleBody->Branch("eventNum",&theEventNum,"eventNum/I");
  ntupleBody->Branch("bodyType",&fBodyType);
  ntupleBody->Branch("bodyStatus",&bodyStatus);
  ntupleBody->Branch("pID",&fPID);
  ntupleBody->Branch("EachDepE",&EachDepE);
  ntupleBody->Branch("mom_x",&mom_x);
  ntupleBody->Branch("mom_y",&mom_y);
  ntupleBody->Branch("mom_z",&mom_z);
  ntupleBody->Branch("gtime",&gtime);
  ntupleBody->Branch("prePos_x",&prePos_x);
  ntupleBody->Branch("prePos_y",&prePos_y);
  ntupleBody->Branch("prePos_z",&prePos_z);
  ntupleBody->Branch("pos_x",&pos_x);
  ntupleBody->Branch("pos_y",&pos_y);
  ntupleBody->Branch("pos_z",&pos_z);
  ntupleBody->Branch("tEnergy",&tEnergy);
  ntupleBody->Branch("trackID",&fTrackID);

  ntupleDecay= new TTree ("ntupleDecay","Tree for MCParticles");
  ntupleDecay->Branch("eventNum",&theEventNum,"eventNum/I");
  ntupleDecay->Branch("beamIndex",&theBeamIndex,"beamIndex/I");
  ntupleDecay->Branch("Dgtime",&Dgtime);
  ntupleDecay->Branch("Dmom_x",&Dmom_x);
  ntupleDecay->Branch("Dmom_y",&Dmom_y);
  ntupleDecay->Branch("Dmom_z",&Dmom_z);
  ntupleDecay->Branch("DtEnergy",&DtEnergy);
  ntupleDecay->Branch("Dpol_x",&Dpol_x);
  ntupleDecay->Branch("Dpol_y",&Dpol_y);
  ntupleDecay->Branch("Dpol_z",&Dpol_z);
  ntupleDecay->Branch("Dpos_x",&Dpos_x);
  ntupleDecay->Branch("Dpos_y",&Dpos_y);
  ntupleDecay->Branch("Dpos_z",&Dpos_z);
  ntupleDecay->Branch("DPDG",&DPDG);
  ntupleDecay->Branch("DtrackID",&DtrackID);
  ntupleDecay->Branch("DparentID",&DparentID);

  ntupleTransport = new TTree("ntupleTransport", "GEANT4 TransportTree");
  ntupleTransport->Branch("eventNum", &theEventNum, "TeventNum/I");
  ntupleTransport->Branch("Tpos_x", &fTpos_x);
  ntupleTransport->Branch("Tpos_y", &fTpos_y);
  ntupleTransport->Branch("Tpos_z", &fTpos_z);
  ntupleTransport->Branch("Tmom_x", &fTmom_x);
  ntupleTransport->Branch("Tmom_y", &fTmom_y);
  ntupleTransport->Branch("Tmom_z", &fTmom_z);
  ntupleTransport->Branch("Ttime", &fTtime);
  ntupleTransport->Branch("TtEnergy", &fTtEnergy);
  ntupleTransport->Branch("Tpol_x", &fTpol_x);
  ntupleTransport->Branch("Tpol_y", &fTpol_y);
  ntupleTransport->Branch("Tpol_z", &fTpol_z);
  ntupleTransport->Branch("Tmag_x", &fTmag_x);
  ntupleTransport->Branch("Tmag_y", &fTmag_y);
  ntupleTransport->Branch("Tmag_z", &fTmag_z);
}

ApplicationManager::~ApplicationManager()
{
  delete theApplication;
}

void ApplicationManager::Open(G4String fileNameString)
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

void ApplicationManager::Save()
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

void ApplicationManager::ClearNtuple() 
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
  fBodyType.clear();
  bodyStatus.clear();
  mom_x.clear();
  mom_y.clear();
  mom_z.clear();
  gtime.clear();
  pos_x.clear();
  pos_y.clear();
  pos_z.clear();
  prePos_x.clear();
  prePos_y.clear();
  prePos_z.clear();
  tEnergy.clear();
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

  //thePrevTime=0.;
  thePrevTime=-1e9*second;

  return;
}

void ApplicationManager::PutDecayValue(G4double DTEnergy, G4ThreeVector Dpos, G4ThreeVector Dmom, G4ThreeVector Dpol,G4double DGtime, G4int PDG, G4int trackID, G4int parentID)
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

void ApplicationManager::PutNtupleValue(G4int parID, G4double TEnergy, G4ThreeVector prePos, G4ThreeVector pos, G4ThreeVector mom, G4double Gtime, G4int bodyType, G4int bodyStat, G4double eachDepE, G4int trackID)
{
  EachDepE.push_back(eachDepE/MeV);
  fBodyType.push_back(bodyType);
  bodyStatus.push_back(bodyStat);
  fPID.push_back(parID);
  mom_x.push_back(mom.x());
  mom_y.push_back(mom.y());
  mom_z.push_back(mom.z());
  gtime.push_back(Gtime);
  prePos_x.push_back(prePos.x());
  prePos_y.push_back(prePos.y());
  prePos_z.push_back(prePos.z());
  pos_x.push_back(pos.x());
  pos_y.push_back(pos.y());
  pos_z.push_back(pos.z());
  tEnergy.push_back(TEnergy);
  fTrackID.push_back(trackID);

  return;
}

void ApplicationManager::PutTransportValue(G4double TtEnergy, G4ThreeVector Tmom, G4double Ttime, G4ThreeVector Tpos, G4ThreeVector Tpol, G4ThreeVector Tmag)
{
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
