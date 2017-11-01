// $Id: MagneticField.cc,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#include "MagneticField.hh"
#include "MagneticFieldMessenger.hh"

#include "G4EqMagElectricField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4ClassicalRK4.hh"
#include "G4SimpleHeum.hh"

#include "TMath.h"

#include "G4SystemOfUnits.hh"

#include <fstream>

MagneticField* MagneticField::object = 0;

MagneticField::MagneticField()
  : G4MagneticField(),pStepper(0),fEquation(0),fEquationSpin(0),fCalType("uniform"),fWithSpin(false)
{
  object = this;
  updateField();
  
  fMagFieldMessenger = new MagneticFieldMessenger(this);
}

void MagneticField::FillFieldValue()
{
  if(fCalType=="interpolation" ||
     fCalType=="interpolationstorage" || 
     fCalType=="strict"){
    // weak focusing field interpolation
    std::ifstream ifs("FLDATA/20160422_Abe2017May-77.txt"); // 5ko model
    G4int IDUM;
    ifs >> fNF;
    for(G4int i=0;i<fNF;++i){
      ifs >> IDUM >> fFLR[i] >> fFLZ[i] >> fFLCRNT[i];
    }
    ifs.close();
    
    fGraph_Bz = new TGraph2D;
    fGraph_Br = new TGraph2D;
    
    G4int count = 0;
    G4double RM, ZM, BR, BZ, APHI;
    G4int rbin = 100;
    G4int zbin = 500;
    G4double roffset = 1e-3; // m
    G4double rmax = 0.5; // m
    G4double zmax = 0.5; // m
    if(fCalType=="interpolationstorage"){
      rbin = 10;
      zbin = 50;
      roffset = 0.293; // m
      rmax = 0.373; // m
      zmax = 0.05; // m
    }
    for(G4int i=0;i<rbin;++i){
      for(G4int j=0;j<=zbin;++j){
	RM = roffset+i*rmax/(G4double)rbin;
	ZM = (2*j-zbin)*zmax/(G4double)zbin;
	bflfit(fNF,fFLR,fFLZ,fFLCRNT,RM,ZM,BR,BZ,APHI);
	fGraph_Bz->SetPoint(count, RM, ZM, BZ);
	fGraph_Br->SetPoint(count, RM, ZM, BR);
	count++;
      }
    }
  }
}

MagneticField::~MagneticField()
{
  delete fMagFieldMessenger;
}

MagneticField* MagneticField::getObject()
{
  if (!object) new MagneticField();
  return object;
}

void MagneticField::updateField()
{

  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  G4TransportationManager* fTransportManager =
         G4TransportationManager::GetTransportationManager();

  fieldPropagator = fTransportManager->GetPropagatorInField();

  fieldMgr->SetDetectorField(this);

  if(pStepper) delete pStepper;

  if(fWithSpin){
    if(fEquationSpin) delete fEquationSpin;
    fEquationSpin = new G4EqEMFieldWithSpin(this);
    pStepper = new G4ClassicalRK4(fEquationSpin,12);
  }else{
    if(fEquation) delete fEquation;//houiken 1007 sasikae
    fEquation = new G4EqMagElectricField(this);
    pStepper = new G4ClassicalRK4(fEquation,8);
  }

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

void MagneticField::GetFieldValue( const G4double Point[4],G4double* Bfield ) const
{
  // Point[0],Point[1],Point[2],Point[3] are x, y, z, t cordinates 

  const G4double Er = 0.*volt/m; // set electroic fieled to zero
  //const G4double Er = 2.113987E+6*volt/m;
  // Er=a*Bz*c*beta/(double)((1-beta*beta)-a*beta*beta);//volt/m
  //a=0.0011659208;
  //gamma=5;
  //beta=sqrt(1-1/(double)pow(gamma,2));

  G4double Ex,Ey;

  // uniform magnetic field
  G4double Bz = 3.0*tesla;
  G4double Br = 0.*tesla;

  G4double Bx,By;
 
  G4double posR=sqrt(pow(Point[0],2)+pow(Point[1],2));
  G4double cos_theta,sin_theta;

  if(fCalType=="interpolation" || fCalType=="interpolationstorage"){
    // interpolated weak focusing magnetic field calculation
    Br = fGraph_Br->Interpolate(posR/m, Point[2]/m)*tesla;
    Bz = fGraph_Bz->Interpolate(posR/m, Point[2]/m)*tesla;
  }else if(fCalType=="strict"){
    // strict weak focusing magnetic field calculation (too slow!)
    Br=0.0;
    Bz=0.0;

    G4double APHI=0.0E0;
    double DDD,DBR,DBZ,DAPHI;
    G4double posRm = posR/m;
    G4double zm = Point[2]/m;

    for(int i=0;i<fNF;++i){
      DDD=sqrt((fFLR[i]-posR)*(fFLR[i]-posR)+(fFLZ[i]-Point[2])*(fFLZ[i]-Point[2]));
      DBR = 0.;
      DBZ = 0.;
      if(DDD!=0.0E0){
	bfield(fFLR[i],fFLZ[i],fFLCRNT[i],posRm,zm,DBR,DBZ,DAPHI);  
      }
      Br=Br+DBR;
      Bz=Bz+DBZ;
      APHI=APHI+DAPHI;
    }
    Br = Br*tesla;
    Bz = Bz*tesla;
  }

  if(posR>0){
     cos_theta=Point[0]/posR;
     sin_theta=Point[1]/posR;
     Ex=-1*Er*cos_theta;
     Ey=-1*Er*sin_theta;
     Bx=Br*cos_theta;
     By=Br*sin_theta;
  }else{
     Ex=0.;
     Ey=0.;
     Bx=0.;
     By=0.;
  }
  
  Bfield[0]=Bx;
  Bfield[1]=By;
  Bfield[2]=Bz;
  Bfield[3]=Ex;
  Bfield[4]=Ey;
  Bfield[5]=0;

  return;
}


void MagneticField::bflfit(int NF,double FLR[500],double FLZ[500],double FLCRNT[500],double RM,double ZM,double &BR,double &BZ,double &APHI)
{
  BR=0.0;
  BZ=0.0;
  APHI=0.0;
  double DDD,DBR,DBZ,DAPHI;
  for(int i=0;i<NF;++i){
    DDD=sqrt(pow(FLR[i]-RM,2)+pow(FLZ[i]-ZM,2));
    if(DDD!=0.0E0){
      bfield(FLR[i],FLZ[i],FLCRNT[i],RM,ZM,DBR,DBZ,DAPHI);  
    }
    BR=BR+DBR;
    BZ=BZ+DBZ;
    APHI=APHI+DAPHI;
  }
}

void MagneticField::bfield(double CR,double CZ,double CI,double RI,double ZJ,double &BR,double &BZ,double &APHI) const
{                      
//      COMMON/COND/ ILL                                                
//      DOUBLE PRECISION    RK,  ELPK,ELPE                              
//                                                                      
//    (CR,CZ)--- POSITION OF COIL                                      
//    (RI,ZJ)--- MEASUREMENT POSITION                                  
//    (BR,BZ)--- MAGNETIC FIELD                                        
//    (APHI) --- VECTOR POTENTIAL                                      
  double XMU=4.E-7;                                                        
  double S =RI*RI+CR*CR+(ZJ-CZ)*(ZJ-CZ);                                  
  double P =RI*CR+RI*CR;                                                  
  double RK=(P+P)/(double)(S+P);  
  double PSI;
  double ELPK,ELPE,ILL; 
  cep12d(RK,ELPK,ELPE,ILL);                                  
  
  if(ILL==0){                                          
    RK=sqrt(RK);                                                    
    BZ =XMU*CI/(double)(2.* sqrt(S+P))*(ELPK-(S-2.*CR*CR)/(double)(S-P)*ELPE);    
    BR =XMU*CI/(double)(2.* sqrt(S+P))*(ZJ-CZ)/(double)RI*(-ELPK+ S/(double)(S-P)*ELPE);    
    PSI=CI/(double)RK* sqrt(RI*CR)*((1.E0-RK*RK/(double)2.)*ELPK-ELPE);            
    APHI=XMU*PSI/(double)RI;                                                  
  }
}         
//---------------------------------------------------------------------  
void MagneticField::cep12d(double RK,/*double I,*/double &AK,double &AE,double &ILL) const
{                                
  /*
    DATA  AZ,A1,A2,A3,A4/  1.38629436112D0,0.09666344259D0,      
    A                           0.03590092383D0,0.03742563713D0,      
    B                           0.01451196212D0 /                      
    DATA  BZ,B1,B2,B3,B4/  0.5D0          ,0.12498593597D0,      
    A                           0.06880248576D0,0.03328355346D0,      
    B                           0.00441787012D0/                      
    DATA EAZ,EA1,EA2,EA3,EA4  /                                  
    A                           1.0D0          ,0.44325141463D0,      
    B                           0.06260601220D0,0.04757383546D0,      
    C                           0.01736506451D0/                      
    DATA EBZ,EB1,EB2,EB3,EB4 /                                    
    A                           0.0D0          ,0.24998368310D0,      
    B                           0.09200180037D0,0.04069697526D0,      
    C                           0.00526449639D0/                      
  */
  double AZ=1.38629436112E0;
  double A1=0.09666344259E0;
  double A2=0.03590092383E0;
  double A3=0.03742563713E0;
  double A4=0.01451196212E0;
  double BZ=0.5E0;
  double B1=0.12498593597E0;
  double B2=0.06880248576E0;
  double B3=0.03328355346E0;
  double B4=0.00441787012E0;
  double EAZ=1.0E0;
  double EA1=0.44325141463E0;
  double EA2=0.06260601220E0;
  double EA3=0.04757383546E0;
  double EA4=0.01736506451E0;
  double EBZ=0.0E0;
  double EB1=0.24998368310E0;
  double EB2=0.09200180037E0;
  double EB3=0.04069697526E0;
  double EB4=0.00526449639E0;
  
  ILL=0;                                                           
  double XM1,XM2,XM3,XM4,DALXM,ALXM;
  double BZZ;
  if(RK<0. || RK>1.){               
    ILL=1;                                                            
    AK=0.0;                                                            
    AE=0.0;                                                            
  }else{
    XM1=1.-RK;                                                      
    XM2=XM1*XM1;                                                      
    XM3=XM2*XM1;                                                      
    XM4=XM3*XM1;                                                      
    DALXM=1./(double)XM1;                                                  
    ALXM=log(DALXM);                                                
    BZZ=BZ+B1*XM1+B2*XM2+B3*XM3+B4*XM4;                                
    AK= AZ+A1*XM1+A2*XM2+A3*XM3+A4*XM4 + BZZ*ALXM;                    
    BZZ=EBZ+EB1*XM1+EB2*XM2+EB3*XM3+EB4*XM4;                      
    AE=EAZ+EA1*XM1+EA2*XM2+EA3*XM3+EA4*XM4+BZZ*ALXM;              
  }
}                                                            
