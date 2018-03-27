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
  : G4MagneticField(),pStepper(0),fEquation(0),fEquationSpin(0),fCalType("uniform"),fFieldFileName(""),fKickerCalType("off"),fKickerFieldFileName(""),fWithSpin(false),
    // weak B-field constants
    fXMU(4.E-7),
    fFLRMin(1.), // m
    fFLZMin(1.), // m
    // kicker constants
    fFacB(2.596),
    fStart_kick(0*ns),
    fEndKickPar(0.5),
    fTK(288.5*ns),
    fTau0(1.e20*ns),
    fTau(1.e29*ns)
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
    G4cout << "opening field file: " << fFieldFileName << G4endl;
    std::ifstream ifs(fFieldFileName.c_str());
    if(!ifs.is_open()){
      G4cerr << "field file: " << fFieldFileName << " cannot be opend!" << G4endl;
      return;
    }
    G4int IDUM;
    ifs >> fNF;
    for(G4int i=0;i<fNF;++i){
      ifs >> IDUM >> fFLR[i] >> fFLZ[i] >> fFLCRNT[i];
      if( fFLR[i]<fFLRMin && abs(fFLZ[i])<fFLZMin ){
	if( fFLR[i]>abs(fFLZ[i]) ){
	  fFLRMin = fFLR[i];
	}else{
	  fFLZMin = abs(fFLZ[i]);
	}
      }
    }
    ifs.close();
    G4cout << "FLRMin = " << fFLRMin << " FLZMin = " << fFLZMin << G4endl;

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
	bflfit(RM,ZM,BR,BZ,APHI);
	fGraph_Bz->SetPoint(count, RM, ZM, BZ);
	fGraph_Br->SetPoint(count, RM, ZM, BR);
	count++;
      }
    }
  }
}

void MagneticField::FillKickerFieldValue()
{
  if(fKickerCalType=="interpolation" ||
    fKickerCalType=="strict"){
    G4cout << "opening kicker field file: " << fKickerFieldFileName << G4endl;
    std::ifstream ifs(fKickerFieldFileName.c_str());
    if(!ifs.is_open()){
      G4cerr << "kicker field file: " << fKickerFieldFileName << " cannot be opend!" << G4endl;
      return;
    }
    char lineread[256];
    for(G4int i=0; i<8; ++i){
      ifs.getline(lineread, 256);
    }
    fKick_spatial_i = 0;
    G4double readx,ready,readz,readBx,readBy,readBz;
    for(G4int i=0; i<200; ++i){
      ifs >> readx >> ready >> readz >> readBx >> readBy >> readBz;
      if(ifs.eof()) break;
      fPosR[i] = readx*cm;
      fPosY[i] = ready*cm;
      fBXk[i]  = readBx;
      fBYk[i]  = readBy;
      fKick_spatial_i = i;
    }
    ifs.close();

    if(fKick_spatial_i>0){
      fRatio = abs(fFacB)/fBXk[0];
    }
    fEnd_kick = fEndKickPar*fTK;
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
  const G4double Ez = 0.*volt/m; 
  //const G4double Er = 2.113987E+6*volt/m;
  // Er=a*Bz*c*beta/(double)((1-beta*beta)-a*beta*beta);//volt/m
  //a=0.0011659208;
  //gamma=5;
  //beta=sqrt(1-1/(double)pow(gamma,2));

  G4double Ex,Ey;

  // uniform magnetic field: fCalType=="uniform"
  G4double Bz = 3.0*tesla;
  G4double Br = 0.*tesla;

  G4double Bx,By;
 
  G4double posR=sqrt(Point[0]*Point[0]+Point[1]*Point[1]);
  G4double cos_theta,sin_theta;

  if(fCalType=="interpolation" || fCalType=="interpolationstorage"){
    // interpolated weak focusing magnetic field calculation
    Br = fGraph_Br->Interpolate(posR/m, Point[2]/m)*tesla;
    Bz = fGraph_Bz->Interpolate(posR/m, Point[2]/m)*tesla;
  }else if(fCalType=="strict"){
    // strict weak focusing magnetic field calculation (too slow!)
    Br=0.0;
    Bz=0.0;

    G4double APHI = 0.;
    G4double posRm = posR/m;
    G4double zm = Point[2]/m;

    bflfit(posRm,zm,Br,Bz,APHI);

    Br*=tesla;
    Bz*=tesla;
  }

  if( fKickerCalType!="off" ){
    if( Point[3]>=fStart_kick && Point[3]<(fStart_kick+fEnd_kick) ){
      G4double BRkick,BYkick;
      G4double BRkick0,BYkick0;
      // Get kick-B-spatial 
      G4double pos[3] = {Point[0], Point[2], Point[1]};
      kickSpatial(pos, BRkick0, BYkick0);
      G4double int_time = Point[3]-fStart_kick; // ns
      // Get kick-B-field
      kickMag(int_time, BRkick0, BYkick0, BRkick, BYkick);
      Br += BRkick;
      Bz += BYkick;
    }
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
  Bfield[5]=Ez;

  return;
}


void MagneticField::bflfit(double RM,double ZM,double &BR,double &BZ,double &APHI) const
{
  BR=0.0;
  BZ=0.0;
  APHI=0.0;
  G4double DBR=0.,DBZ=0.,DAPHI=0.;

  if( RM<fFLRMin && abs(ZM)<fFLZMin ){
    for(G4int i=0; i<fNF;++i){
      bfield(fFLR[i],fFLZ[i],fFLCRNT[i],RM,ZM,DBR,DBZ,DAPHI);
      BR += DBR;
      BZ += DBZ;
      APHI += DAPHI;
    }
  }else{
    G4double DDD;
    for(G4int i=0;i<fNF;++i){
      DDD = (fFLR[i]-RM)*(fFLR[i]-RM)+(fFLZ[i]-ZM)*(fFLZ[i]-ZM);
      if(DDD!=0.){
	bfield(fFLR[i],fFLZ[i],fFLCRNT[i],RM,ZM,DBR,DBZ,DAPHI);  
      }
      BR += DBR;
      BZ += DBZ;
      APHI += DAPHI;
    }
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
  //double XMU=4.E-7;
  G4double S =RI*RI+CR*CR+(ZJ-CZ)*(ZJ-CZ);                                  
  G4double P =2*RI*CR;                                                  
  G4double RK=(P+P)/(double)(S+P);  
  //G4double PSI;
  G4double ELPK,ELPE;
  cep12d(RK,ELPK,ELPE);
  
  RK=sqrt(RK);                                                    
  BZ =fXMU*CI/(2.* sqrt(S+P))*(ELPK-(S-2.*CR*CR)/(S-P)*ELPE);    
  BR =fXMU*CI/(2.* sqrt(S+P))*(ZJ-CZ)/RI*(-ELPK+ S/(S-P)*ELPE);    
  //PSI=CI/RK*sqrt(RI*CR)*((1.-0.5*RK*RK)*ELPK-ELPE);            
  //APHI=fXMU*PSI/RI;
  APHI=fXMU*CI*sqrt(RI*CR)*((1.-0.5*RK*RK)*ELPK-ELPE)/(RK*RI);
}         
//---------------------------------------------------------------------  
void MagneticField::cep12d(double RK,double &AK,double &AE) const
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

  double XM1,XM2,XM3,XM4,ALXM;
  double BZZ;

  // RK>0. && RK<1. is satisfied by definition
  XM1=1.-RK;                                                      
  XM2=XM1*XM1;                                                      
  XM3=XM2*XM1;                                                      
  XM4=XM3*XM1;  
  ALXM=-log(XM1);                                                
  BZZ=BZ+B1*XM1+B2*XM2+B3*XM3+B4*XM4;                                
  AK= AZ+A1*XM1+A2*XM2+A3*XM3+A4*XM4 + BZZ*ALXM;                    
  BZZ=EBZ+EB1*XM1+EB2*XM2+EB3*XM3+EB4*XM4;                      
  AE=EAZ+EA1*XM1+EA2*XM2+EA3*XM3+EA4*XM4+BZZ*ALXM;              
}                                                            


void MagneticField::kickSpatial(const G4double x[3], G4double &BRkick0, G4double &BYkick0) const
{
  G4int ff=0;
  G4int ff2;
  G4double min=1E10;
  G4double dis;
  G4double tmpy;
  for(G4int i=0;i<fKick_spatial_i;++i){
    tmpy=abs(x[1]);
    dis=(tmpy-fPosY[i])*(tmpy-fPosY[i]);
    if(dis<min){
      //printf("fPosY[%d]=%lf min=%lf\n",ff,fPosY[ff],min);
      min=dis;
      ff=i;
    }
  }
  if(ff>0 && ff<(fKick_spatial_i-1)){
    G4double min2=(tmpy-fPosY[ff+1])*(tmpy-fPosY[ff+1]);
    G4double min3=(tmpy-fPosY[ff-1])*(tmpy-fPosY[ff-1]);
    ff2=ff+1;
    if(min2>=min3)ff2=ff-1;
  }else if(ff==0){
    ff2=ff+1;
  }else if(ff==(fKick_spatial_i-1)){
    ff2=ff-1;
  }else{
    G4cout << "No kicker component is close to particle" << G4endl;
    BRkick0 = 0.;
    BYkick0 = 0.;
    return;
  }
  BRkick0=(fBXk[ff]+fBXk[ff2])*0.5*fRatio;
  BYkick0=(fBYk[ff]+fBYk[ff2])*0.5*fRatio;
}


void MagneticField::kickMag(const G4double t, const G4double BRkick0, const G4double BYkick0, G4double &BRkick, G4double &BYkick) const
{
  G4double BRkickTemp=BRkick0*fFacB*gauss;
  G4double BYkickTemp=BYkick0*0*gauss;
  G4double theta=(t/fTK)*CLHEP::twopi;

  if(t<fTau0) BRkick = BRkickTemp*sin(theta)*exp(-t/fTau);
  else BRkick = BRkickTemp*sin(theta)*exp(-(t-fTau0)/fTau);

  BYkick=BYkickTemp*sin(theta);
}
