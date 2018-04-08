// $Id: BodySD.cc,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#include "BodySD.hh"
#include "ApplicationManager.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4ProcessTable.hh"// Need to get "procName" Hiromi 2009/08/22
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4TouchableHandle.hh"
#include "G4ThreeVector.hh"
#include "G4Proton.hh"

#include "CalHit.hh"


#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TFile.h" 
#include "TTree.h" 

BodySD::BodySD()
  : G4VSensitiveDetector("body")
{ collectionName.insert("calorimeter");}

BodySD::~BodySD()
{}

/////////////////////////////////////////////////////
void BodySD::Initialize(G4HCofThisEvent* /*HCTE*/)
/////////////////////////////////////////////////////
{
  /*
  // create hit collection(s)
  hitsCollection= new CalHitsCollection(SensitiveDetectorName,
                                        collectionName[0]); 
  
  // push H.C. to "Hit Collection of This Event"
  G4int hcid= GetCollectionID(0);
  HCTE-> AddHitsCollection(hcid, hitsCollection);
  */

  // clear energy deposit buffer
  //for (G4int i=0; i<NCHANNEL; i++) edepbuf[i]=0.;

  currentTrackID = 0;
  currentTotalDepE = 0.;
  unsummedDepE = 0.;
  steplengthTotal = 0.;
}
/////////////////////////////////////

G4bool BodySD::ProcessHits(G4Step* aStep, G4TouchableHistory*/* ROhist*/)
{
  ApplicationManager* application = ApplicationManager::GetApplicationManager();

  const G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  const G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  const G4StepStatus pointIn = preStepPoint->GetStepStatus();
  const G4StepStatus pointOut = postStepPoint->GetStepStatus();
  //See details manual page 275 FAQ.4. Tracks and steps

  //////***
  // get step information from "PreStepPoint"
  //G4TouchableHistory* touchable= (G4TouchableHistory*)(preStepPoint-> GetTouchable());

  G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();

  //////***

  G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
  G4Track* track = aStep->GetTrack();
  G4ThreeVector deltaposition = aStep->GetDeltaPosition();
  G4double steplength = aStep->GetStepLength();
  G4double totaledep = aStep->GetTotalEnergyDeposit();
  G4VPhysicalVolume* physicalvolume = track->GetVolume();
  G4String volumename = physicalvolume->GetName();
  G4LogicalVolume* logicalvolume = physicalvolume->GetLogicalVolume();
  G4Material* material = logicalvolume->GetMaterial();
  G4String materialname = material->GetName();

  G4double gtime,tE;
  G4ThreeVector mom;
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  int bodyType = 0;
  int bodyStatus = 0;

  if(volumename=="sensor"){
    bodyType = 1;
    mom = track->GetMomentum();
    gtime = track->GetGlobalTime();
    tE = track->GetTotalEnergy();
    
    G4int id= bodyType;
    if(id<0)id=0;
    if(id>=2000)id=1999;
    //edepbuf[id]+= pos.x()/mm;

    G4int trackID = track->GetTrackID();
    if( fHitTrackID.find(trackID)==fHitTrackID.end() ){
      fHitTrackID.insert(trackID);
      application->PutDecayValue(tE, preStepPoint->GetPosition(), mom, track->GetPolarization(), gtime, pd->GetPDGEncoding(), trackID, track->GetParentID());
    }

    switch(pointIn){
    case fGeomBoundary:
    case fAlongStepDoItProc:
    case fUndefined:
      bodyStatus += 0x01;
      unsummedDepE = totaledep;
      if( currentTrackID!=0 ) G4cout << "Warning: trackID is not reset" << G4endl;
      currentTrackID = trackID;
      break;
    default:
      unsummedDepE += totaledep;
      break;
    }
    currentTotalDepE += totaledep;
    steplengthTotal += steplength;

    switch(pointOut){
    case fGeomBoundary:
    case fAlongStepDoItProc:
      bodyStatus += 0x02;
      if( currentTrackID!=trackID ) G4cout << "Warning: different track" << G4endl;
      currentTrackID = 0;
      break;
    default:
      break;
    }

    if(bodyStatus > 0 && unsummedDepE>0.){
      application->PutNtupleValue(pd->GetPDGEncoding(), tE, preStepPoint->GetPosition(), postStepPoint->GetPosition(), mom, gtime, bodyType, bodyStatus, unsummedDepE, trackID);
      unsummedDepE = 0.;
    }
  }
  
  return true;
}
/////////////////////////////////////////////////
void BodySD::EndOfEvent(G4HCofThisEvent* )
/////////////////////////////////////////////////
{
  ApplicationManager* application = ApplicationManager::GetApplicationManager();
  application->SetTotalEdep(currentTotalDepE);
  application->SetTotalStepLength(steplengthTotal);

  unsummedDepE = 0.;
  currentTotalDepE = 0.;
  steplengthTotal = 0.;

  fHitTrackID.clear();

  // make hits and push them to "Hit Coleltion"
  /*
    for (G4int id=0; id< NCHANNEL; id++) {
    if(edepbuf[id] > 0. ) {
    CalHit* ahit= new CalHit(id, edepbuf[id]);
    hitsCollection-> insert(ahit);
    }
    }*/
}

/////////////////////////////
void BodySD::DrawAll()
/////////////////////////////
{
}

//////////////////////////////
void BodySD::PrintAll()
//////////////////////////////
{
  hitsCollection-> PrintAllHits();
}
