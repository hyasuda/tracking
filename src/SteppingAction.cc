#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "SteppingActionMessenger.hh"
#include "G4Step.hh"
#include "ApplicationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4StepPoint.hh"
#include "G4StepStatus.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ProcessTable.hh"
#include "G4RadioactiveDecay.hh"
#include "G4DecayProducts.hh"
#include "G4Delete.hh"
#include "G4RunManager.hh"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"


SteppingAction::SteppingAction(DetectorConstruction* det,
			       EventAction* evt)
  :detector(det), eventaction(evt), fTimeStep(1.*ns) 
{
  fMessenger = new SteppingActionMessenger(this);
}


SteppingAction::~SteppingAction()
{
  delete fMessenger;
}


void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  ApplicationManager* application = 
  ApplicationManager::GetApplicationManager();

  //Get Muon Decay Information
  //procname DecayWithSpin, StepLimiter,Transportation,SynRad
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  G4Track* track= aStep->GetTrack();
  G4double gtime_all= track->GetGlobalTime();//nsec

  //G4cout << "time = " << gtime_all << " procName = " << procName << G4endl;

  if(gtime_all>40000){
    G4cout << "--------------------------g_time :" << gtime_all  << G4endl;
    track->SetTrackStatus(fStopAndKill);
  }

  if( fSaveStep ){
    if(gtime_all-application->GetPrevTime()>=fTimeStep){
      //G4cout << "gtime = " << gtime_all << " muon r = " << track->GetPosition().perp() << " z = " << track->GetPosition().z() << " pz' = " << track->GetMomentum().z()/track->GetMomentum().mag() << G4endl;
      if( gtime_all-application->GetPrevTime()>=2*fTimeStep && application->GetPrevTime()>0. ) G4cout << "there is missed step" << G4endl;
      application->SetPrevTime((G4int)(gtime_all/fTimeStep)*fTimeStep);
      
      if(track->GetDefinition()->GetParticleName()=="mu+"){
	MagneticField *mymagField = MagneticField::getObject();
	G4double point[4] = {track->GetPosition().x(),track->GetPosition().y(),track->GetPosition().z(),gtime_all};
	G4double Bfield[6];
	mymagField->GetFieldValue(point, Bfield);
	G4ThreeVector Bmag(Bfield[0],Bfield[1],Bfield[2]);
	application->PutTransportValue(track->GetTotalEnergy(), track->GetMomentum(), gtime_all, track->GetPosition(), track->GetPolarization(), Bmag);
      }
    }
  }

  if(track->GetDefinition()->GetParticleName()=="e+"){
    if( track->GetParentID()==application->GetPositronID() ){
      //G4cout << "New positron ID = " << track->GetTrackID() << " parentID = " << track->GetParentID() << G4endl;
      application->SetPositronID(track->GetTrackID());
    }
  } 

  if(procName == "DecayWithSpin" ){
     track= aStep->GetTrack();
     gtime_all= track->GetGlobalTime();//nsec

     G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
     G4String particlename = pd->GetParticleName();


     printf("######################################\n");
     G4cout << "procname " << procName << G4endl;
     G4int stepid = track->GetCurrentStepNumber();
     G4cout << "particlename " << particlename << G4endl;
     G4cout << "stepid " << stepid << G4endl;
     printf("gtime=%f \n",gtime_all);

     //Get Information
     G4ThreeVector pol ;
     G4ThreeVector mom ;
     G4double Gtime ;//nsec
     G4ThreeVector pos ;
     G4double TE;

     eventNum=application->GetEventNum();
     pol = track->GetPolarization();
     mom = track->GetMomentum();
     Gtime = track->GetGlobalTime();//nsec
     pos = track->GetPosition();
     TE= track->GetTotalEnergy();
     
     application->PutDecayValue(TE/MeV, pos, mom, pol, Gtime, track->GetParticleDefinition()->GetPDGEncoding(), track->GetTrackID(), track->GetParentID());
     application->SetPositronID( track->GetTrackID() );

     G4TrackVector *secondary =fpSteppingManager->GetfSecondary();
     G4TrackVector::const_iterator p = secondary->begin();
     for ( ; p != secondary->end(); ++p) {
       G4ParticleDefinition *particle = (*p)->GetDefinition();
       G4String particlenameDaughter = particle->GetParticleName();

       mom = (*p)->GetMomentum();
       TE= (*p)->GetTotalEnergy();
       pol = (*p)->GetPolarization();
       application->PutDecayValue(TE/MeV, pos, mom, pol, Gtime, particle->GetPDGEncoding(), (*p)->GetTrackID(), (*p)->GetParentID());
     }//END for ( ; p != secondary->end(); ++p) {

  }else if(procName=="eBrem" || procName == "eIoni" || procName=="annihil" || procName=="msc"
                  || procName=="compt" || procName == "phot" || procName=="conv" || procName=="Rayl" ) {
    /*track= aStep->GetTrack();
    gtime_all= track->GetGlobalTime();//nsec

    G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
    G4String particlename = pd->GetParticleName();

    //Get Information
    //G4double KE;
    G4ThreeVector pol ;
    //G4ThreeVector momv;
    G4ThreeVector mom ;
    //G4double Ptime ;//nsec
    //G4double Gtime ;//nsec
    G4ThreeVector pos ;//nsec
    //G4double TE;
 
    eventNum=application->GetEventNum();
    KE = aStep->GetPreStepPoint()->GetKineticEnergy()/MeV;
    pol = track->GetPolarization();
    momv = track->GetMomentumDirection();
    mom = track->GetMomentum();
    //Ptime = track->GetProperTime();//nsec
    //Gtime = track->GetGlobalTime();//nsec
    pos = track->GetPosition();//nsec
    //TE= track->GetTotalEnergy();
 
    G4TrackVector *secondary =fpSteppingManager->GetfSecondary();
    G4TrackVector::const_iterator p = secondary->begin();
    int count=0;
    G4double Egamma=0; 
    G4double Eelectron=0; 
    for ( ; p != secondary->end(); ++p) {
      G4ParticleDefinition *particle = (*p)->GetDefinition();
      G4String particlenameDaughter = particle->GetParticleName();
      
      KE = (*p)->GetKineticEnergy()/MeV;
      momv =(*p)->GetMomentumDirection();
      mom = (*p)->GetMomentum();
      //TE= (*p)->GetTotalEnergy();
      //Ptime = (*p)->GetProperTime();//nsec
      momv = (*p)->GetMomentumDirection();//nsec
      count++; 
      if(particlenameDaughter=="gamma"){
	Egamma=Egamma+KE;
      }
      else if(particlenameDaughter=="e-"){
	Eelectron=Eelectron+KE;
      }
      }*/
  }else if(procName!="StepLimiter" && procName!="Transportation"){
    G4cout << "procName = " << procName << G4endl;
  }
}


