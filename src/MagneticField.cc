// $Id: MagneticField.cc,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#include "MagneticField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4ClassicalRK4.hh"
#include "TMath.h"

#include "G4SystemOfUnits.hh"

MagneticField* MagneticField::object = 0;

MagneticField::MagneticField()
  : G4MagneticField(), pStepper(0), fEquation(0)
{
  object = this;
  updateField();
}

MagneticField::~MagneticField()
{;}

MagneticField* MagneticField::getObject()
{
  if (!object) new MagneticField();
  return object;
}

void MagneticField::updateField()
{

  if(fEquation) delete fEquation;//houiken 1007 sasikae
  fEquation = new G4EqEMFieldWithSpin(this);
  //fEquation = new G4Mag_SpinEqRhs(this);

  //apply a global uniform magnetic field along Z axis

  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  G4TransportationManager* fTransportManager =
         G4TransportationManager::GetTransportationManager();

  fieldPropagator = fTransportManager->GetPropagatorInField();


  fieldMgr->SetDetectorField(this);

  if(pStepper) delete pStepper;
  pStepper = new G4ClassicalRK4(fEquation,12);

  //  fieldMgr->CreateChordFinder(this);

  //  Create a cord finder providing the (global field, min step length,
  //  a pointer to the stepper)
  G4double minStep=0.01*mm;
  G4double deltaChord=3.0*mm;
  G4double deltaOneStep=0.01*mm;
  G4double deltaIntersection=0.1*mm;
  G4double epsMin=2.5e-7*mm;
  G4double epsMax=0.05*mm;

  fChordFinder = new G4ChordFinder((G4MagneticField*)this,minStep,pStepper);
 
  // Set accuracy parameters
  fChordFinder->SetDeltaChord( deltaChord );
 
  fieldMgr->SetAccuraciesWithDeltaOneStep(deltaOneStep);
 
  fieldMgr->SetDeltaIntersection(deltaIntersection);
 
  fieldPropagator->SetMinimumEpsilonStep(epsMin);
  fieldPropagator->SetMaximumEpsilonStep(epsMax);
 
  /*
  G4cout << "Accuracy Parameters:" <<
             " MinStep=" << minStep <<
             " DeltaChord=" << deltaChord <<
             " DeltaOneStep=" << deltaOneStep << G4endl;
  G4cout << "                    " <<
             " DeltaIntersection=" << deltaIntersection <<
             " EpsMin=" << epsMin <<
             " EpsMax=" << epsMax <<  G4endl;
  */ 

  fieldMgr->SetChordFinder(fChordFinder);

}

void MagneticField::GetFieldValue( const G4double Point[3],G4double* Bfield ) const
{
  // Point[0],Point[1],Point[2] are x-, y-, z-cordinates 

  const G4double Bz = 3.0*tesla;//3.0*tesla;
  const G4double Er = 0.*volt/m; // set electroic fieled to zero
  //const G4double Er = 2.113987E+6*volt/m;
  // Er=a*Bz*c*beta/(double)((1-beta*beta)-a*beta*beta);//volt/m
  //a=0.0011659208;
  //gamma=5;
  //beta=sqrt(1-1/(double)pow(gamma,2));

  //double pi=TMath::Pi();
  double Ex,Ey;
 
  double posR=sqrt(pow(Point[0],2)+pow(Point[1],2));
  double cos_theta,sin_theta;
  if(posR>0){
     cos_theta=Point[0]/(double)posR;
     sin_theta=Point[1]/(double)posR;
     Ex=-1*Er*cos_theta;//apply radial electric field
     Ey=-1*Er*sin_theta;
  }else{
     Ex=0;
     Ey=0;
  }
  

  Bfield[0]=0;
  Bfield[1]=0;
  Bfield[2]=Bz;
  Bfield[3]=Ex;
  Bfield[4]=Ey;
  Bfield[5]=0;

  return;
}
