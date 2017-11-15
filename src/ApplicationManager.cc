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
  : theEdepByEvent(), theEdepByRun(), theHitPosition(), 
    theFileStream(), 
    theApplication(0), 
    theEdepHist(0), theHitHist(0)
{
  if ( theApplicationManager ) {
    G4cout <<  "ApplicationManager constructed twice." << G4endl;
    exit(0);
  }
  theApplicationManager = this;

  theApplication = new TApplication("Geant4 User Application", 0, 0);

  theEdepHist = 
    new TH1D("theEdepHist", "#DeltaE by Step in Body", 600, 0.0, 300.0);
  theEdepHist->GetXaxis()->SetTitle("#DeltaE (MeV)");
  theEdepHist->GetXaxis()->SetNoExponent();
  theEdepHist->GetYaxis()->SetTitle("Number of events");
  theEdepHist->SetFillColor(kBlue);



  // const G4ThreeVector BODY_SIZE(40*cm, 60*cm, 20*cm);
  G4double bodyx = 40*cm;
  G4double bodyy = 60*cm;
  G4double pitch = 1*cm;
  G4int nbinx = G4int(bodyx/pitch);
  G4int nbiny = G4int(bodyy/pitch);

  theHitHist = new TH2D("theHitHist", 
                        "Lateral Profile", 
                        nbinx, -bodyx/2.0, bodyx/2.0, 
                        nbiny, -bodyy/2.0, bodyy/2.0);
  gErrorIgnoreLevel = 3000;

  ntupleBody= new TTree ("ntupleBody","GEANT4 BodyTree");
  ntupleBody->Branch("eventNum",&fEventNum,"eventNum/I");
  //ntupleBody->Branch("hitInfo",&fHitInfo,"hitInfo/I");

  ntupleBody->Branch("bodyTyp",&bodyTyp);
  ntupleBody->Branch("bodyStatus",&bodyStatus);
  //ntupleBody->Branch("chID",&chID);
  ntupleBody->Branch("pID",&fPID);
  //ntupleBody->Branch("CurrentDepE",&CurrentDepE);
  ntupleBody->Branch("EachDepE",&EachDepE);
  //ntupleBody->Branch("kEnergy",&kEnergy);
  ntupleBody->Branch("mom_x",&mom_x);
  ntupleBody->Branch("mom_y",&mom_y);
  ntupleBody->Branch("mom_z",&mom_z);
  //ntupleBody->Branch("ptime",&ptime);
  ntupleBody->Branch("gtime",&gtime);
  ntupleBody->Branch("pos_x",&pos_x);
  ntupleBody->Branch("pos_y",&pos_y);
  ntupleBody->Branch("pos_z",&pos_z);
  ntupleBody->Branch("tEnergy",&tEnergy);
  ntupleBody->Branch("isPrimary",&fIsPrimary);
  ntupleBody->Branch("trackID",&fTrackID);
  //ntupleBody->Branch("stepLength",&fStepLength);
  //ntupleBody->Branch("stepLengthTotal",&fStepLengthTotal);

  ntupleDecay= new TTree ("ntupleDecay","GEANT4 DecayTree");
  ntupleDecay->Branch("eventNum",&fEventNum,"eventNum/I");
  //ntupleDecay->Branch("Dptime",&Dptime,"Dptime/F");
  ntupleDecay->Branch("Dgtime",&Dgtime,"Dgtime/F");
  // ntupleDecay->Branch("DkEnergy",&DkEnergy,"DkEnergy[4]/F");
  ntupleDecay->Branch("Dmom_x",&Dmom_x,"Dmom_x[4]/F");
  ntupleDecay->Branch("Dmom_y",&Dmom_y,"Dmom_y[4]/F");
  ntupleDecay->Branch("Dmom_z",&Dmom_z,"Dmom_z[4]/F");
  //ntupleDecay->Branch("Dmomv_x",&Dmomv_x,"Dmomv_x[4]/F");
  //ntupleDecay->Branch("Dmomv_y",&Dmomv_y,"Dmomv_y[4]/F");
  //ntupleDecay->Branch("Dmomv_z",&Dmomv_z,"Dmomv_z[4]/F");
  ntupleDecay->Branch("Dpol_x",&Dpol_x,"Dpol_x/F");
  ntupleDecay->Branch("Dpol_y",&Dpol_y,"Dpol_y/F");
  ntupleDecay->Branch("Dpol_z",&Dpol_z,"Dpol_z/F");
  ntupleDecay->Branch("Dpos_x",&Dpos_x,"Dpos_x/F");
  ntupleDecay->Branch("Dpos_y",&Dpos_y,"Dpos_y/F");
  ntupleDecay->Branch("Dpos_z",&Dpos_z,"Dpos_z/F");
  ntupleDecay->Branch("DtEnergy",&DtEnergy,"DtEnergy[4]/F");
  ntupleDecay->Branch("DPDG",&DPDG,"DPDG[4]/I");
}

ApplicationManager::~ApplicationManager()
{
  delete theEdepHist;
  delete theHitHist;
  delete theApplication;
}
