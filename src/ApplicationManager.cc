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

  gErrorIgnoreLevel = 3000;

  ntupleBody= new TTree ("ntupleBody","Tree for MCSteps and SimHits");
  ntupleBody->Branch("eventNum",&theEventNum,"eventNum/I");
  ntupleBody->Branch("bodyTyp",&bodyTyp);
  ntupleBody->Branch("bodyStatus",&bodyStatus);
  ntupleBody->Branch("pID",&fPID);
  ntupleBody->Branch("EachDepE",&EachDepE);
  ntupleBody->Branch("mom_x",&mom_x);
  ntupleBody->Branch("mom_y",&mom_y);
  ntupleBody->Branch("mom_z",&mom_z);
  ntupleBody->Branch("gtime",&gtime);
  ntupleBody->Branch("pos_x",&pos_x);
  ntupleBody->Branch("pos_y",&pos_y);
  ntupleBody->Branch("pos_z",&pos_z);
  ntupleBody->Branch("tEnergy",&tEnergy);
  ntupleBody->Branch("isPrimary",&fIsPrimary);
  ntupleBody->Branch("trackID",&fTrackID);

  ntupleDecay= new TTree ("ntupleDecay","Tree for MCParticles");
  ntupleDecay->Branch("eventNum",&theEventNum,"eventNum/I");
  ntupleDecay->Branch("beamIndex",&theBeamIndex,"beamIndex/I");
  ntupleDecay->Branch("Dgtime",&Dgtime);
  ntupleDecay->Branch("Dmom_x",&Dmom_x);
  ntupleDecay->Branch("Dmom_y",&Dmom_y);
  ntupleDecay->Branch("Dmom_z",&Dmom_z);
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
  ntupleTransport->Branch("eventNum", &fEventNum, "TeventNum/I");
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
