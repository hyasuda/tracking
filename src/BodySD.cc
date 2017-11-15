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
int chID;

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
  G4int eventNum=application->GetEventNum();
  G4int  hitInfo=application->GetHitInfo();
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  /*
  pos = track->GetPosition();
  G4double ktmpx = pos.x();
  G4double ktmpy = pos.y();
  G4double ktmpz = pos.z();
  */
  G4int pID=0;
  /*std::ofstream fcpnum("fcopyNo.dat",std::ios::app);
  fcpnum << pID << " " << copyNo << " " << motherCopyNo << " " << ktmpx << " " << ktmpy << " " << ktmpz << G4endl;
  fcpnum.close();*/

  bodyTyp = 201;
  
  /*bodyTyp=-1;
  if(volumename=="Frame")bodyTyp=100; 
  if(volumename=="testMaterial")bodyTyp=101; 
  if(volumename=="BoxDetector")bodyTyp=102;//No material 
  else bodyTyp=201;
  if(volumename=="panel1")bodyTyp=201;//Si 
  if(volumename=="panel1a")bodyTyp=301;//Si 
  if(volumename=="panel1b")bodyTyp=401;//Si 
  if(volumename=="panel1c")bodyTyp=501;//Si 
  if(volumename=="panel1d")bodyTyp=601;//Si 
  if(volumename=="panel1e")bodyTyp=701;//Si 
  
  if(volumename=="panel2")bodyTyp=202;//Si 
  if(volumename=="panel2a")bodyTyp=302;//Si 
  if(volumename=="panel2b")bodyTyp=402;//Si 
  if(volumename=="panel2c")bodyTyp=502;//Si 
  if(volumename=="panel2d")bodyTyp=602;//Si 
  if(volumename=="panel2e")bodyTyp=702;//Si 
  
  if(volumename=="panel3")bodyTyp=203;//Si 
  if(volumename=="panel3a")bodyTyp=303;//Si 
  if(volumename=="panel3b")bodyTyp=403;//Si 
  if(volumename=="panel3c")bodyTyp=503;//Si 
  if(volumename=="panel3d")bodyTyp=603;//Si 
  if(volumename=="panel3e")bodyTyp=703;//Si 
  
  if(volumename=="panel4")bodyTyp=204;//Si 
  if(volumename=="panel4a")bodyTyp=304;//Si 
  if(volumename=="panel4b")bodyTyp=404;//Si 
  if(volumename=="panel4c")bodyTyp=504;//Si 
  if(volumename=="panel4d")bodyTyp=604;//Si 
  if(volumename=="panel4e")bodyTyp=704;//Si 
  
  if(volumename=="panel5")bodyTyp=205;//Si 
  if(volumename=="panel5a")bodyTyp=305;//Si 
  if(volumename=="panel5b")bodyTyp=405;//Si 
  if(volumename=="panel5c")bodyTyp=505;//Si 
  if(volumename=="panel5d")bodyTyp=605;//Si 
  if(volumename=="panel5e")bodyTyp=705;//Si 
  
  if(volumename=="panel6")bodyTyp=206;//Si 
  if(volumename=="panel6a")bodyTyp=306;//Si 
  if(volumename=="panel6b")bodyTyp=406;//Si 
  if(volumename=="panel6c")bodyTyp=506;//Si 
  if(volumename=="panel6d")bodyTyp=606;//Si 
  if(volumename=="panel6e")bodyTyp=706;//Si 
  
  if(volumename=="panel7")bodyTyp=207;//Si 
  if(volumename=="panel7a")bodyTyp=307;//Si 
  if(volumename=="panel7b")bodyTyp=407;//Si 
  if(volumename=="panel7c")bodyTyp=507;//Si 
  if(volumename=="panel7d")bodyTyp=607;//Si 
  if(volumename=="panel7e")bodyTyp=707;//Si 
  
  if(volumename=="panel8")bodyTyp=208;//Si 
  if(volumename=="panel8a")bodyTyp=308;//Si 
  if(volumename=="panel8b")bodyTyp=408;//Si 
  if(volumename=="panel8c")bodyTyp=508;//Si 
  if(volumename=="panel8d")bodyTyp=608;//Si 
  if(volumename=="panel8e")bodyTyp=708;//Si 
  
  if(volumename=="CenterW")bodyTyp=1000;//Calorimeter 
  if(volumename=="CenterD"){
    bodyTyp=1010;//LeadTangstate 
    //  printf("ppp\n");getchar();
  }
  if(volumename=="CenterInP")bodyTyp=1020;//InnerPipe
  */

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
    //edepbuf[id]+= aStep-> GetTotalEnergyDeposit();
    edepbuf[id]+= pos.x()/mm;
    
    application->SetHitPosition(pos);
    application->SetHitMomentum(mom);
    application->SetHitKEnergy(kE);
    application->SetHitTEnergy(tE);
    application->SetHitGtime(gtime);
    application->SetHitPtime(ptime);
    
    application->AddEdepByEvent(totaledep);
    application->AddEdepByRun(totaledep);
    
    application->Fill(pos.x()/mm, pos.y()/mm, totaledep/MeV);

    G4int isPrimary = 0;
    G4int trackID = track->GetTrackID();
    //G4double CurrentDepE = application->GetEdepByEvent();

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
      application->PutNtupleValue(pID,kE/MeV, tE/MeV, pos/mm, mom, gtime, ptime, bodyTyp, bodyStatus, chID, eventNum, hitInfo, currentTotalDepE/MeV,unsummedDepE/MeV,isPrimary,trackID);
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

