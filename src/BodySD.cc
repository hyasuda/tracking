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

int bodyTyp;
int bodyStatus;

BodySD::BodySD()
  : G4VSensitiveDetector("body")
{ collectionName.insert("calorimeter");}

BodySD::~BodySD()
{;}

/////////////////////////////////////////////////////
void BodySD::Initialize(G4HCofThisEvent* HCTE)
/////////////////////////////////////////////////////
{
  // create hit collection(s)
  hitsCollection= new CalHitsCollection(SensitiveDetectorName,
                                        collectionName[0]); 
  
  // push H.C. to "Hit Collection of This Event"
  G4int hcid= GetCollectionID(0);
  HCTE-> AddHitsCollection(hcid, hitsCollection);

  // clear energy deposit buffer
  for (G4int i=0; i<NCHANNEL; i++) edepbuf[i]=0.;

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
  //G4int copyNo = theTouchable->GetReplicaNumber();
  //G4int motherCopyNo = theTouchable->GetReplicaNumber(1);
  G4Track* track = aStep->GetTrack();
  //G4int stepid = track->GetCurrentStepNumber();
  G4String particlename = pd->GetParticleName();
  G4ThreeVector deltaposition = aStep->GetDeltaPosition();
  G4double steplength = aStep->GetStepLength();
  G4double totaledep = aStep->GetTotalEnergyDeposit();
  G4VPhysicalVolume* physicalvolume = track->GetVolume();
  G4String volumename = physicalvolume->GetName();
  G4LogicalVolume* logicalvolume = physicalvolume->GetLogicalVolume();
  G4Material* material = logicalvolume->GetMaterial();
  G4String materialname = material->GetName();
  //G4double density = material->GetDensity();

  G4double ptime,gtime,tE,kE;
  G4ThreeVector mom,pos;
  G4int  hitInfo=application->GetHitInfo();
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  G4int pID=0;

  bodyTyp = 201;
  
  bodyStatus=0;

  if(volumename=="sensor"){
    if( pointIn==fGeomBoundary || pointIn==fAlongStepDoItProc || pointIn==fUndefined ) bodyStatus += 0x01;
    if( pointOut==fGeomBoundary || pointOut==fAlongStepDoItProc ) bodyStatus += 0x02;

    hitInfo++;
    
    mom = track->GetMomentum();
    pos = track->GetPosition();
    ptime = track->GetProperTime();//nsec
    gtime = track->GetGlobalTime();//nsec
    tE= track->GetTotalEnergy();
    kE = track->GetKineticEnergy();
    
    G4int id= bodyTyp;
    if(id<0)id=0;
    if(id>=2000)id=1999;
    edepbuf[id]+= pos.x()/mm;
    
    application->SetHitPosition(pos);
    application->SetHitMomentum(mom);
    application->SetHitKEnergy(kE);
    application->SetHitTEnergy(tE);
    application->SetHitGtime(gtime);
    application->SetHitPtime(ptime);
    
    application->AddEdepByEvent(totaledep);
    application->AddEdepByRun(totaledep);
    
    G4int isPrimary = 0;
    G4int trackID = track->GetTrackID();

    if(particlename=="e-") pID=11;
    else if(particlename=="e+"){
      pID=-11;
      if( track->GetTrackID()==application->GetPositronID() ){
	isPrimary = 1;
      }
    }else if(particlename=="gamma") pID=22;
    
    if(pointIn==fGeomBoundary || pointIn==fUndefined || pointIn==fAlongStepDoItProc){
      currentTotalDepE = totaledep;
      unsummedDepE = totaledep;
      steplengthTotal = steplength;
      //if( currentTrackID!=0 ) G4cout << "trackID is not reset" << G4endl;
      currentTrackID = trackID;
    }
    else{
      currentTotalDepE += totaledep;
      unsummedDepE += totaledep;
      steplengthTotal += steplength;
    }
    if(pointOut==fGeomBoundary || pointOut==fAlongStepDoItProc){
      if( currentTrackID!=trackID ) G4cout << "different track" << G4endl;
      currentTrackID = 0;
    }   
    
    if(bodyStatus > 0 && unsummedDepE>0.){
      application->PutNtupleValue(pID,tE/MeV, pos/mm, mom, gtime, bodyTyp, bodyStatus, hitInfo, unsummedDepE/MeV,isPrimary,trackID);
      unsummedDepE = 0.;
    }
  }
  
  application->SetHitInfo(hitInfo);

  return true;
}
/////////////////////////////////////////////////
void BodySD::EndOfEvent(G4HCofThisEvent* )
/////////////////////////////////////////////////
{
  // make hits and push them to "Hit Coleltion"
  for (G4int id=0; id< NCHANNEL; id++) {
    if(edepbuf[id] > 0. ) {
      CalHit* ahit= new CalHit(id, edepbuf[id]);
      hitsCollection-> insert(ahit);
    }
  }
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
