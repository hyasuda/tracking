#include "EventAction.hh"
#include "ApplicationManager.hh"

#include "RunAction.hh"
#include "EventActionMessenger.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"

#include "BodySD.hh"

#include "Randomize.hh"
#include <iomanip>

#include "TROOT.h"
#include "TH1.h"



EventAction::EventAction(RunAction* run)
:runAct(run),printModulo(1),eventMessenger(0)
{
  eventMessenger = new EventActionMessenger(this);
}


EventAction::~EventAction()
{
  delete eventMessenger;
}


void EventAction::BeginOfEventAction(const G4Event* evt)
{  
 G4int evtNb = evt->GetEventID();
 extern G4int DokodemoBango;
  DokodemoBango=evtNb;
 extern G4int PickBegin;
  PickBegin=0;
 if (evtNb%printModulo == 0) { 
   G4cout << "\n---> Begin of event: " << evtNb << G4endl;
   CLHEP::HepRandom::showEngineStatus();
 }
 
 // initialisation per event
 EnergyAbs = EnergyGap = 0.;
 TrackLAbs = TrackLGap = 0.;

//DataBroker 
 ApplicationManager* application = 
 ApplicationManager::GetApplicationManager();

 application->SetEdepByEvent(0.0);

 application->SetEventNum(evtNb);
 application->ClearNtuple(evtNb);

///DataBrokerEND///////////////////////
}

void EventAction::EndOfEventAction(const G4Event* evt)
{

  G4SDManager* SDManager= G4SDManager::GetSDMpointer();

  // get "Hit Collection of This Event"
  G4HCofThisEvent* HCTE= evt-> GetHCofThisEvent();
  if(! HCTE) return;

  // get a hit collection
  static G4int idcal= -1;
  if(idcal<0)  idcal= SDManager-> GetCollectionID("calorimeter");
  CalHitsCollection* hccal= (CalHitsCollection*)HCTE-> GetHC(idcal);
  if (!hccal) return; // no hit collection

  // get hits
  G4int nhits= hccal-> entries();
  for(G4int idx=0; idx< nhits; idx++) {
    //G4int ich= (*hccal)[idx]-> GetID();
    //G4double edep= (*hccal)[idx]-> GetEdep();

  }

  //accumulates statistic
  //
  runAct->fillPerEvent(EnergyAbs, EnergyGap, TrackLAbs, TrackLGap);



  //print per event (modulo n)
  //
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) {
    G4cout << "---> End of event: " << evtNb << G4endl;	

    G4cout
       << "   Absorber: total energy: " << std::setw(7)
                                        << G4BestUnit(EnergyAbs,"Energy")
       << "       total track length: " << std::setw(7)
                                        << G4BestUnit(TrackLAbs,"Length")
       << G4endl
       << "        Gap: total energy: " << std::setw(7)
                                        << G4BestUnit(EnergyGap,"Energy")
       << "       total track length: " << std::setw(7)
                                        << G4BestUnit(TrackLGap,"Length")
       << G4endl;
	  
  }  
//DataBroker 
  ApplicationManager* application = 
  ApplicationManager::GetApplicationManager();

  G4ThreeVector pos = application->GetHitPosition();

  //G4double hitE = application->GetHitTEnergy();

  G4double edep = application->GetEdepByEvent();

  const G4String tab = "\t";

  int hitInfo=application->GetHitInfo();
  int eventNum=-1;//application->GetEventNum();
  application->FillDecayNtuple();
  application->FillNtuple();
  application->FillTransportNtuple();
  application->Fill(edep/MeV);
  printf("EndOfEventAction eventNum=%d hitInfo=%d dep=%lf\n",eventNum,hitInfo,edep/MeV);
  application->SetBeamIndex(-1);

  application->Update();
///DataBrokerEND///////////////////////

}  


