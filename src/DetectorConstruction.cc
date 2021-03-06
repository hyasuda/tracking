#include <vector>

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
//#include "G4Sphere.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4SubtractionSolid.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"//To apply steplength upper limit hiromi
#include "BodySD.hh"//To apply sensitive body hiromi
#include "G4SDManager.hh"//To apply sensitive body hiromi

#include "G4ClassicalRK4.hh"

#include "MagneticField.hh"
#include "TMath.h"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

DetectorConstruction::DetectorConstruction(const G4int nvane,const G4bool doStrip)
  :defaultMaterial(0),fNvane(nvane),fDoStrip(doStrip)
   //magField(0), mymagField(0)
{
  // materials
  DefineMaterials();

  // ensure the global field is initialized
  (void)MagneticField::getObject();

  // create commands for interactive definition of the calorimeter
  detectorMessenger = new DetectorMessenger(this);
}


DetectorConstruction::~DetectorConstruction()
{ delete detectorMessenger;}

/*
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
  }*/


void DetectorConstruction::DefineMaterials()
{ 
 //This function illustrates the possible ways to define materials
 
G4String symbol;             //a=mass of a mole;
G4double a, z, density;      //z=mean number of protons;  
G4int iz, n;                 //iz=number of protons  in an isotope; 
                             // n=number of nucleons in an isotope;

G4int ncomponents, natoms;
G4double abundance, fractionmass;


//
// define Elements
//
G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
G4Element* Si = new G4Element("Silicon" ,symbol="Si", z= 14., a= 28.09*g/mole);
G4Element* Pb = new G4Element("Lead"    ,symbol="Pb", z=82., a= 207.19*g/mole);
G4Element* W  = new G4Element("Tangstate",symbol="W", z=74., a= 183.84*g/mole);
G4Element* Ni = new G4Element("Nikkel"  ,symbol="Ni", z=28., a= 58.69*g/mole);
G4Element* Cu = new G4Element("Copper"  ,symbol="Cu", z=29., a= 63.55*g/mole);  
//G4Element* S  = new G4Element("Tangstate",symbol="S", z=16., a= 183.84*g/mole);


//
// define an Element from isotopes, by relative abundance 
//
G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);
G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
U->AddIsotope(U5, abundance= 90.*perCent);
U->AddIsotope(U8, abundance= 10.*perCent);


//
// define simple materials
//
//G4Material* Al = new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.699*g/cm3);
//G4Material* LAr = new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
G4Material* PurePb = new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
//G4Material* Fe = new G4Material("Fe", z= 26., a= 55.83*g/mole, density = 7.874*g/cm3);
//Fe;7874 kg/m3, 7.09E-6 m3/mol
G4Material* PureSi = new G4Material("Si", z= 14., a= 28.09*g/mole, density = 2.33*g/cm3);
//G4Material* W = new G4Material("Tangstate", z=74., a= 183.84*g/mole, density= 19.3*g/cm3);
G4Material* ArgonGas= new G4Material("ArgonGas", z=18., a= 39.95*g/mole, density= (1.66*1E-3)*g/cm3);


//
// define a material from elements.   case 1: chemical molecule
//
G4Material* H2O = 
new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
H2O->AddElement(H, natoms=2);
H2O->AddElement(O, natoms=1);
// overwrite computed meanExcitationEnergy with ICRU recommended value 
H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);

G4Material* N2m4 =
  new G4Material("N2m4", density=1.12e-12*g/cm3, ncomponents=1, kStateGas, 
		 300.*kelvin, 1.e-4*pascal);
 N2m4->AddElement(N, natoms=2);

G4Material* N2m3 =
  new G4Material("N2m3", density=1.12e-11*g/cm3, ncomponents=1, kStateGas, 
		 300.*kelvin, 1.e-3*pascal);
 N2m3->AddElement(N, natoms=2);

G4Material* N2m2 =
  new G4Material("N2m2", density=1.12e-10*g/cm3, ncomponents=1, kStateGas, 
		 300.*kelvin, 1.e-2*pascal);
 N2m2->AddElement(N, natoms=2);

G4Material* N2m1 =
  new G4Material("N2m1", density=1.12e-9*g/cm3, ncomponents=1, kStateGas, 
		 300.*kelvin, 1.e-1*pascal);
 N2m1->AddElement(N, natoms=2);

G4Material* N2m0 =
  new G4Material("N2m0", density=1.12e-8*g/cm3, ncomponents=1, kStateGas, 
		 300.*kelvin, 1.*pascal);
 N2m0->AddElement(N, natoms=2);

G4Material* Sci = 
new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
Sci->AddElement(C, natoms=9);
Sci->AddElement(H, natoms=10);

G4Material* PbWO4 = 
new G4Material("LeadTungsten", density= 8.28*g/cm3, ncomponents=3);
PbWO4->AddElement(Pb, natoms=1);
PbWO4->AddElement(W, natoms=1);
PbWO4->AddElement(O, natoms=4);

G4Material* CH4 = 
new G4Material("Ethane", density= (1.263*1E-3)*g/cm3, ncomponents=2);
CH4->AddElement(C, natoms=2);
CH4->AddElement(H, natoms=6);

G4Material* TPCGAS = 
new G4Material("TPCGAS", density= (1.58*1E-3)*g/cm3, ncomponents=2);
TPCGAS->AddMaterial(ArgonGas, fractionmass=10*perCent);
TPCGAS->AddMaterial(CH4, fractionmass=90*perCent);

G4Material* Dyneema =//TOYOBO http://www.toyobo.co.jp/seihin/dn/dyneema/meigara/index.htm 
new G4Material("Dyneema", density= (0.97)*g/cm3, ncomponents=2);
Dyneema->AddElement(C, natoms=2);
Dyneema->AddElement(H, natoms=4);

G4Material* PSF =//Poly Sulfone http://www.kda1969.com/pla_material/pla_mate_psf2.htm 
new G4Material("PSF", density= (1.2)*g/cm3, ncomponents=3);
PSF->AddElement(C, natoms=27);
PSF->AddElement(H, natoms=38);
PSF->AddElement(O, natoms=4);
//PSF->AddElement(S, natoms=1);

G4Material* Fiber = 
new G4Material("ScintillatingFiber", density= 1.056*g/cm3, ncomponents=2);
Fiber->AddElement(C, natoms=6);
Fiber->AddElement(H, natoms=5);

G4Material* Myl = 
new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
Myl->AddElement(C, natoms=10);
Myl->AddElement(H, natoms= 8);
Myl->AddElement(O, natoms= 4);

G4Material* SiO2 = 
new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
SiO2->AddElement(Si, natoms=1);
SiO2->AddElement(O , natoms=2);

G4Material* Polycarbonate = 
new G4Material("Lexan",density= 1.200*g/cm3, ncomponents=3);
Polycarbonate->AddElement(C, natoms=16);
Polycarbonate->AddElement(H , natoms=14);
Polycarbonate->AddElement(O , natoms=3);

G4Material* Polymethylmethacrylate = 
new G4Material("acrylic",density= 1.19*g/cm3, ncomponents=3);
Polymethylmethacrylate->AddElement(C, natoms=5);
Polymethylmethacrylate->AddElement(H , natoms=8);
Polymethylmethacrylate->AddElement(O , natoms=2);

G4Material* LeadTangstate = 
new G4Material("LeadTan",density= 8.300*g/cm3, ncomponents=3);
LeadTangstate->AddElement(Pb, natoms=1);
LeadTangstate->AddElement(W , natoms=1);
LeadTangstate->AddElement(O , natoms=4);

G4Material* Epoxy = new G4Material("Epoxy", density= 1.7*g/cm3 ,ncomponents=3);
Epoxy->AddElement(C, natoms=10);
Epoxy->AddElement(H, natoms=10);
Epoxy->AddElement(O, natoms= 2);
 
// Tungsten heavy alloy
 G4Material* HAC2 = new G4Material("HAC2", density=17.9*g/cm3, ncomponents=3);
 HAC2->AddElement(W, natoms=94);
 HAC2->AddElement(Ni, natoms=4);
 HAC2->AddElement(Cu, natoms=2);

 G4Material* HAC1 = new G4Material("HAC1", density=18.5*g/cm3, ncomponents=3);
 HAC1->AddElement(W, natoms=97);
 HAC1->AddElement(Ni, natoms=2);
 HAC1->AddElement(Cu, natoms=1);


// CRFP (Carbon Fiber Reinforced Polymer): M55 Quasiisotropic Layup
// Taken from geant4 advanced example cosmicray_charging
G4Material* CFRP = new G4Material("CFRP", density= 1.66*g/cm3, 1);
CFRP->AddElement(C,1);


//
// define a material from elements.   case 2: mixture by fractional mass
//
G4Material* Air = 
new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
Air->AddElement(N, fractionmass=0.7);
Air->AddElement(O, fractionmass=0.3);

G4Material* WPb = 
new G4Material("WPb", density= 19.30*g/cm3, ncomponents=2);
WPb->AddElement(W, fractionmass=99.9*perCent);
WPb->AddElement(Pb, fractionmass=0.1*perCent);

G4Material* Kapton = new G4Material("Kapton", density= 1.42*g/cm3, ncomponents=4);
Kapton->AddElement(H, fractionmass = 0.0273);
Kapton->AddElement(C, fractionmass = 0.7213);
Kapton->AddElement(N, fractionmass = 0.0765);
Kapton->AddElement(O, fractionmass = 0.1749);


//
// define a material from elements and/or others materials (mixture of mixtures)
//
G4Material* Aerog = 
new G4Material("Aerogel", density= 0.200*g/cm3, ncomponents=3);
Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
Aerog->AddElement (C   , fractionmass= 0.1*perCent);

G4Material* G10_Plate = 
new G4Material("G10_Plate", density= 1.7*g/cm3, ncomponents=2);
G10_Plate->AddMaterial(SiO2, fractionmass= 60*perCent);
G10_Plate->AddMaterial(Epoxy, fractionmass=40*perCent);


//
// examples of gas in non STP conditions
//
G4Material* CO2 = 
new G4Material("CarbonicGas", density= 27.*mg/cm3, ncomponents=2,
                              kStateGas, 325.*kelvin, 50.*atmosphere);
CO2->AddElement(C, natoms=1);
CO2->AddElement(O, natoms=2);
 
G4Material* steam = 
new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
                             kStateGas, 500.*kelvin, 2.*atmosphere);
steam->AddMaterial(H2O, fractionmass=1.);


//
// examples of vacuum
//
G4Material* Vacuum =
new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 2.73*kelvin, 3.e-18*pascal);

G4Material* beam = 
new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                       kStateGas, STP_Temperature, 2.e-2*bar);
beam->AddMaterial(Air, fractionmass=1.);


G4cout << *(G4Material::GetMaterialTable()) << G4endl;


//default materials of the World
 defaultMaterial = Vacuum;
 sensorMat = PureSi;
 subMat = G10_Plate;
 tubeMat = PurePb;
 shieldMat = PurePb;
 //storageGas = N2m2;
 //storageGas = N2m3;
 //storageGas = N2m4;
 //storageGas = N2m1;
 //storageGas = N2m0;
 storageGas = Vacuum;
 fpcMat = Kapton;
 frameMat = CFRP;
 windowMat = Kapton;
}


//G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
G4VPhysicalVolume* DetectorConstruction::Construct()
{

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the Calor parameters definition
  //ComputeCalorParameters();
  
 
  //     
  // World
  //
  WorldSize=1.0*m;
  
  solidWorld = new G4Box("World",				//its name
			 0.5*WorldSize, 0.5*WorldSize, 0.5*WorldSize);	//its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   defaultMaterial,	//its material
                                   "World");		//its name
  
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 logicWorld,		//its logical volume				 
                                 "World",		//its name
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  

  //
  // Another box
  //
  solidFrame = new G4Box("Frame",
                   0.5*WorldSize*0.95, 0.5*WorldSize*0.95, 0.5*WorldSize*0.95);
                       
  logicFrame = new G4LogicalVolume(solidFrame,
                                   storageGas,
				   "Frame");		
                                   
  physiFrame = new G4PVPlacement(0,			
  				 G4ThreeVector(),	
                                 logicFrame,
                                 "Frame",		
                                 logicWorld,		
                                 false,			
                                 0);		


  //
  // virtual vane
  //

  const G4double vvaneW = 208.*mm;
  const G4double vvaneT = 6.*mm;
  const G4double vvaneH = 490.5*mm;
  const G4double r_vane = 82.*mm+0.5*vvaneW;
  
  sol_vvane = new G4Box("vvane", 0.5*vvaneW, 0.5*vvaneT, 0.5*vvaneH);
  log_vvaneUpper = new G4LogicalVolume(sol_vvane,
				       defaultMaterial,
				       "vvaneUpper");
  log_vvaneLower = new G4LogicalVolume(sol_vvane,
				       defaultMaterial,
				       "vvaneLower");
  for(int ii=0; ii<fNvane; ii++){
    G4double rangle = 2.*pi/(G4double)fNvane*ii*radian;
    G4RotationMatrix *rotvu = new G4RotationMatrix();
    rotvu->rotateZ(-rangle);
    phys_vvaneUpper = new G4PVPlacement(rotvu,
					G4ThreeVector(r_vane*cos(rangle),
						      r_vane*sin(rangle),
						      0.5*vvaneH),
					log_vvaneUpper,
					"vvaneUpper",
					logicFrame,
					false,
					ii);
    G4RotationMatrix *rotvd = new G4RotationMatrix();
    rotvd->rotateX(pi*radian);
    rotvd->rotateZ(rangle);
    phys_vvaneLower = new G4PVPlacement(rotvd,
					G4ThreeVector(r_vane*cos(rangle),
						      r_vane*sin(rangle),
						      -0.5*vvaneH),
					log_vvaneLower,
					"vvaneLower",
					logicFrame,
					false,
					ii);
  }

  
  //
  // --- silicon strip sensor
  //

  const G4double sensorW = 98.77*mm;
  const G4double sensorT = 0.32*mm;
  const G4double sensorH = 98.77*mm;
  const G4double deadspace = 0.5*mm;

  G4double subW2 = 17.*mm;
  G4double subH2 = 2.*sensorH + 4.*deadspace; 
  
  G4double subW = 217.5*mm;
  G4double subT = 1.6*mm;
  G4double subH = vvaneH - subH2; 
  
  
  const G4double xcenter = 8.96*mm + sensorW + 0.5*deadspace - 0.5*vvaneW; 
  const G4double zcenter = sensorH + deadspace - 0.5*vvaneH;   
  const G4double splusd  = 0.5*sensorW + 0.5*deadspace;

  const G4double frameW = subH2;
  const G4double frameT = 3.*mm;
  const G4double frameH = subH2;

  const G4double sensorActiveW = 97.28*mm;
  const G4double sensorActiveH = 48.39*mm;
  const G4double splusdActive = 0.5*sensorActiveH + 0.5*deadspace;

  const G4double stripW = 0.19*mm;
  const G4double stripT = 0.32*mm;
  const G4double stripH = 48.39*mm;

  G4int ix=0; 
  G4int iy=0; 
  G4int iz=0; 
  G4int iu=0;
  
  if(fDoStrip){
    sol_sensor = new G4Box("silicon", 0.5*sensorActiveW, 0.5*sensorT, 0.5*sensorActiveH);
    log_sensor = new G4LogicalVolume(sol_sensor,
				     defaultMaterial,
				     "silicon");

    sol_strip = new G4Box("sensor", 0.5*stripW, 0.5*stripT, 0.5*stripH);
    log_strip = new G4LogicalVolume(sol_strip, sensorMat, "sensor");
    phys_strip = new G4PVReplica("sensor",
				 log_strip,
				 log_sensor,
				 kXAxis,
				 512,
				 stripW);
    G4VSolid *inactiveSensorOut = new G4Box("inactiveSensorOut", 0.5*sensorW, 0.5*sensorT, 0.5*sensorH);
    G4VSolid *inactiveSensorIn = new G4Box("inactiveSensorIn", 0.5*sensorActiveW, 0.5*sensorT+0.1*mm, 0.5*sensorActiveH);
    G4VSolid *inactiveSensorTmp = new G4SubtractionSolid("inactiveSensorTmp", inactiveSensorOut, inactiveSensorIn, 0, G4ThreeVector(0., 0., splusdActive));
    sol_inactiveSensor = new G4SubtractionSolid("inactiveSensor", inactiveSensorTmp, inactiveSensorIn, 0, G4ThreeVector(0., 0., -splusdActive));
    
    log_inactiveSensor = new G4LogicalVolume(sol_inactiveSensor,
					     sensorMat,
					     "inactiveSensor");
    G4String physSensorName = "silicon";
    for (int ii=0; ii<8; ii++){
      if (ii%2==0){ ix = 1;} else { ix = -1;} 
      if ((ii/2)%2==0){ iz = 1;} else { iz = -1;} 
      if ((ii/4)%2==0){ iu = 1;} else { iu = -1;}
      
      // R strip
      phys_sensor = new G4PVPlacement(0,
				      G4ThreeVector(xcenter + ix*splusd,
						    0.5*frameT,
						    zcenter + iz*splusd + iu*splusdActive), 
				      log_sensor,
				      physSensorName,
				      log_vvaneUpper,
				      false,
				      0);
      
      phys_sensor = new G4PVPlacement(0,
				      G4ThreeVector(xcenter + ix*splusd,
						    -0.5*frameT,
						    zcenter + iz*splusd + iu*splusdActive), 
				      log_sensor,
				      physSensorName,
				      log_vvaneLower,
				      false,
				      0);
      if( iu==1 ){
	phys_inactiveSensor = new G4PVPlacement(0,
						G4ThreeVector(xcenter + ix*splusd,
							      0.5*frameT,
							      zcenter + iz*splusd),
						log_inactiveSensor,
						"inactiveSensor",
						log_vvaneUpper,
						false,
						0);
	phys_inactiveSensor = new G4PVPlacement(0,
						G4ThreeVector(xcenter + ix*splusd,
							      -0.5*frameT,
							      zcenter + iz*splusd),
						log_inactiveSensor,
						"inactiveSensor",
						log_vvaneLower,
						false,
						0);
      }

      // Z trip
      G4RotationMatrix *rot = new G4RotationMatrix();
      rot->rotateY(0.5*pi);
      phys_sensor = new G4PVPlacement(rot,
				      G4ThreeVector(xcenter + ix*splusd + iu*splusdActive,
						    -0.5*frameT,
						    zcenter + iz*splusd), 
				      log_sensor,
				      physSensorName,
				      log_vvaneUpper,
				      false,
				      0);
      
      phys_sensor = new G4PVPlacement(rot,
				      G4ThreeVector(xcenter + ix*splusd + iu*splusdActive,
						    0.5*frameT,
						    zcenter + iz*splusd), 
				      log_sensor,
				      physSensorName,
				      log_vvaneLower,
				      false,
				      0);
      
      if( iu==1 ){
	phys_inactiveSensor = new G4PVPlacement(rot,
						G4ThreeVector(xcenter + ix*splusd,
							      -0.5*frameT,
							      zcenter + iz*splusd),
						log_inactiveSensor,
						"inactiveSensor",
						log_vvaneUpper,
						false,
						0);
	phys_inactiveSensor = new G4PVPlacement(rot,
						G4ThreeVector(xcenter + ix*splusd,
							      0.5*frameT,
							      zcenter + iz*splusd),
						log_inactiveSensor,
						"inactiveSensor",
						log_vvaneLower,
						false,
						0);
      }
    }
  }else{
    sol_sensor = new G4Box("sensor", 0.5*sensorW, 0.5*sensorT, 0.5*sensorH);
    log_sensor = new G4LogicalVolume(sol_sensor,
				     sensorMat,
				     "sensor");
    G4String physSensorName = "sensor";
    for (int ii=0; ii<8; ii++){
      if (ii%2==0){ ix = 1;} else { ix = -1;}
      if ((ii/2)%2==0){ iy = 1;} else { iy = -1;}
      if ((ii/4)%2==0){ iz = 1;} else { iz = -1;} 
      
      phys_sensor = new G4PVPlacement(0,
				      G4ThreeVector(xcenter + ix*splusd,
						    0.5*iy*frameT,
						    zcenter + iz*splusd), 
				      log_sensor,
				      physSensorName,
				      log_vvaneUpper,
				      false,
				      0);
      
      phys_sensor = new G4PVPlacement(0,
				      G4ThreeVector(xcenter + ix*splusd,
						    0.5*iy*frameT,
						    zcenter + iz*splusd), 
				      log_sensor,
				      physSensorName,
				      log_vvaneLower,
				      false,
				      0);
    }
  }

  //
  // -- FPC
  //

  G4double fpcW = 2.*(sensorW + deadspace);
  G4double fpcT = 0.1*mm;
  G4double fpcH = subH2;

  G4double fpc2W = subW;
  G4double fpc2T = 0.125*mm;
  G4double fpc2H = 80.*mm;

  sol_fpc = new G4Box("fpc", 0.5*fpcW, 0.5*fpcT, (0.5*fpcH-1.*mm)); // temporary
  log_fpc = new G4LogicalVolume(sol_fpc,
				fpcMat,
				"fpc");
  sol_fpc2 = new G4Box("fpc2", 0.5*fpc2W, 0.5*fpc2T, 0.5*fpc2H);
  log_fpc2 = new G4LogicalVolume(sol_fpc2,
				 fpcMat,
				 "fpc2");
  for(G4int ii=0; ii<2; ii++){
    if(ii%2==0){iy=1;}else{iy=-1;}

    phys_fpc = new G4PVPlacement(0,
				 G4ThreeVector(xcenter,
					       iy*0.5*(frameT + sensorT + fpcT),
					       (zcenter+1.*mm)), // temporary 
				 log_fpc,
				 "fpc",
				 log_vvaneUpper,
				 false,
				 0);
    phys_fpc = new G4PVPlacement(0,
				 G4ThreeVector(xcenter,
					       iy*0.5*(frameT + sensorT + fpcT),
					       (zcenter+1.*mm)), // temporary 
				 log_fpc,
				 "fpc",
				 log_vvaneLower,
				 false,
				 0);
    phys_fpc2 = new G4PVPlacement(0,
				  G4ThreeVector(0.*mm,
						iy*0.5*(frameT + sensorT + fpcT),
						(zcenter+sensorH+deadspace+0.5*fpc2H)), 
				  log_fpc2,
				  "fpc2",
				  log_vvaneUpper,
				  false,
				  0);
    phys_fpc2 = new G4PVPlacement(0,
				  G4ThreeVector(0.*mm,
						iy*0.5*(frameT + sensorT + fpcT),
						(zcenter+sensorH+deadspace+0.5*fpc2H)), 
				  log_fpc2,
				  "fpc2",
				  log_vvaneLower,
				  false,
				  0);
  }
  

  //
  // --- Frame
  //

  G4VSolid *out = new G4Box("out", 0.5*frameW, 0.5*frameT, 0.5*frameH);
  G4VSolid *in1 = new G4Box("in1", 93.8*0.5*mm, (0.5*frameT+0.5*mm), 98.8*0.5*mm);
  G4VSolid *in2 = new G4Box("in2", 93.8*0.5*mm, (0.5*frameT+0.5*mm), 98.8*0.5*mm);
  G4VSolid *in3 = new G4Box("in3", 93.8*0.5*mm, (0.5*frameT+0.5*mm), 98.8*0.5*mm);
  G4VSolid *in4 = new G4Box("in4", 93.8*0.5*mm, (0.5*frameT+0.5*mm), 98.8*0.5*mm);
  G4VSolid *tmp_frame1 = new G4SubtractionSolid("frame", out, in1, 0, G4ThreeVector(splusd,0.,splusd));
  G4VSolid *tmp_frame2 = new G4SubtractionSolid("frame", tmp_frame1, in2, 0, G4ThreeVector(splusd,0.,-splusd));
  G4VSolid *tmp_frame3 = new G4SubtractionSolid("frame", tmp_frame2, in3, 0, G4ThreeVector(-splusd,0.,splusd));
  sol_frame = new G4SubtractionSolid("frame", tmp_frame3, in4, 0, G4ThreeVector(-splusd,0.,-splusd));
  
  log_frame = new G4LogicalVolume(sol_frame,
				  frameMat, 
				  "frame");
  phys_frame = new G4PVPlacement(0,
				 G4ThreeVector(xcenter,
					       0.*mm,
					       zcenter),
				 log_frame,
				 "frame",
				 log_vvaneUpper,
				 false,
				 0);

  phys_frame = new G4PVPlacement(0,
				 G4ThreeVector(xcenter,
					       0.*mm,
					       zcenter),
				 log_frame,
				 "frame",
				 log_vvaneLower,
				 false,
				 0);


  //
  // --- SliT128B, etc., just for fun
  //

  const G4double asicW = 7.*mm;
  const G4double asicT = 0.5*mm;
  const G4double asicH = 15.*mm;

  sol_asic = new G4Box("asic", 0.5*asicW, 0.5*asicT, 0.5*asicH);
  log_asic = new G4LogicalVolume(sol_asic,
				 subMat, 
				 "asic");
  
  int jj=0;
  for (int ii=0; ii<64; ii++){
    jj = ii%32;
    if (ii%32 > 15) jj = (ii%32)-16;
    if((ii/32)%2==0){iy=1;}else{iy=-1;}
    phys_asic = new G4PVPlacement(0,
				  G4ThreeVector((-0.5*vvaneW + (jj+1)*12.*mm),
						iy*0.5*(frameT + sensorT + fpcT + asicT),
						(zcenter+sensorH+deadspace+0.5*fpc2H - 13.*((ii/16)%2)*mm)), 
				  log_asic,
				  "asic",
				  log_vvaneUpper,
				  false,
				  0);
    phys_asic = new G4PVPlacement(0,
				  G4ThreeVector((-0.5*vvaneW + (jj+1)*12.*mm),
						iy*0.5*(frameT + sensorT + fpcT + asicT),
						(zcenter+sensorH+deadspace+0.5*fpc2H - 13.*((ii/16)%2)*mm)), 
				  log_asic,
				  "asic",
				  log_vvaneLower,
				  false,
				  0);
  }
  

  const G4double fpgaW = 40.*mm;
  const G4double fpgaT = 3.*mm;
  const G4double fpgaH = 40.*mm;

  sol_fpga = new G4Box("fpga", 0.5*fpgaW, 0.5*fpgaT, 0.5*fpgaH);
  log_fpga = new G4LogicalVolume(sol_fpga,
				 subMat, 
				 "fpga");
  for(G4int ii=0; ii<2; ii++){
    if( ii%2==0 ){ iy=1; }else{ iy=-1; }
    phys_fpga = new G4PVPlacement(0,
				  G4ThreeVector(0.*mm,
						iy*0.5*(frameT + sensorT + fpcT + fpgaT),
						(zcenter+sensorH+deadspace+fpc2H+35.*mm)), 
				  log_fpga,
				  "fpga",
				  log_vvaneUpper,
				  false,
				  0);
    phys_fpga = new G4PVPlacement(0,
				  G4ThreeVector(0.*mm,
						iy*0.5*(frameT + sensorT + fpcT + fpgaT),
						(zcenter+sensorH+deadspace+fpc2H+35.*mm)), 
				  log_fpga,
				  "fpga",
				  log_vvaneLower,
				  false,
				  0);
  }

  G4double sfpW = 10.*mm;
  G4double sfpT = 10.*mm;
  G4double sfpH = 70.*mm;


  sol_sfp = new G4Box("sfp", 0.5*sfpW, 0.5*sfpT, 0.5*sfpH);
  log_sfp = new G4LogicalVolume(sol_sfp,
				subMat, 
				"sfp");
  for(G4int ii=0; ii<2; ii++){
    if( ii%2==0 ){ iy=1; }else{ iy=-1; }
    phys_sfp = new G4PVPlacement(0,
				 G4ThreeVector(0.25*subW,
					       iy*0.5*(frameT + sensorT + fpcT + sfpT),
					       (zcenter+sensorH+deadspace+fpc2H+35.*mm)), 
				 log_sfp,
				 "sfp",
				 log_vvaneUpper,
				 false,
				 0);
    phys_sfp = new G4PVPlacement(0,
				 G4ThreeVector(0.25*subW,
					       iy*0.5*(frameT + sensorT + fpcT + sfpT),
					       (zcenter+sensorH+deadspace+fpc2H+35.*mm)), 
				 log_sfp,
				 "sfp",
				 log_vvaneLower,
				 false,
				 0);
  }
  

  //
  // --- G10 sustrate
  //
  sol_sub = new G4Box("sub", 0.5*subW, 0.5*subT, 0.5*subH);
  log_sub = new G4LogicalVolume(sol_sub,
				subMat,
				"sub");
  phys_sub = new G4PVPlacement(0,
			       G4ThreeVector(-0.5*(vvaneW-subW),0,100*mm),
			       log_sub,
			       "sub",
			       log_vvaneUpper,
			       false,
			       0);
  phys_sub = new G4PVPlacement(0,
			       G4ThreeVector(-0.5*(vvaneW-subW),0,100*mm),
			       log_sub,
			       "sub",
			       log_vvaneLower,
			       false,
			       0);

  sol_sub2 = new G4Box("sub2", 0.5*subW2, 0.5*subT, 0.5*subH2);
  log_sub2 = new G4LogicalVolume(sol_sub2,
				 subMat, // shoud be same as frame
				 "sub2");
  phys_sub2 = new G4PVPlacement(0,
				G4ThreeVector((xcenter - deadspace - sensorW - 0.5*subW2),0,zcenter),
				log_sub2,
				"sub2",
				log_vvaneUpper,
				false,
				0);
  phys_sub2 = new G4PVPlacement(0,
				G4ThreeVector((xcenter - deadspace - sensorW - 0.5*subW2),0,zcenter),
				log_sub2,
				"sub2",
				log_vvaneLower,
				false,
				0);


  //
  // --- Center tube
  //
  
  G4double tube_din = 120.*mm;
  G4double tube_dout = 130.*mm;
  G4double tubeH = 440.*mm;

  sol_tube = new G4Tubs("tube",0.5*tube_din,0.5*tube_dout,0.5*tubeH,
			0.*deg,360.*deg);
  log_tube = new G4LogicalVolume(sol_tube,
				 tubeMat,
				 "tube");
  phys_tube = new G4PVPlacement(0,
				G4ThreeVector(),
				log_tube,
				"tube",
				logicFrame,
				false,
				0);

  //
  // --- Center shield
  //
  
  const G4double shield_din = 58.5*mm;
  const G4double shield_dout = 64.5*mm;
  const G4double shieldH = 400.*mm;

  sol_shield = new G4Tubs("shield",shield_din,shield_dout,0.5*shieldH,
			  0.*deg,360.*deg);
  log_shield = new G4LogicalVolume(sol_shield,
				   shieldMat,
				   "shield");
  phys_shield = new G4PVPlacement(0,
				  G4ThreeVector(),
				  log_shield,
				  "shield",
				  logicFrame,
				  false,
				  0);

  //
  // --- polyimide window
  //

  const G4double window_din = 305.*mm;
  const G4double window_dout = 305.1*mm;
  const G4double windowH = 200.*mm;

  sol_window = new G4Tubs("window",window_din,window_dout,windowH,
			  0.*deg,360.*deg);
  log_window = new G4LogicalVolume(sol_window,
				   windowMat,
				   "window");
  phys_window = new G4PVPlacement(0,
				  G4ThreeVector(),
				  log_window,
				  "window",
				  logicFrame,
				  false,
				  0);
  


  BodySD* bodySD = new BodySD();
  G4SDManager::GetSDMpointer()->AddNewDetector(bodySD);
  if(fDoStrip){
    log_strip->SetSensitiveDetector(bodySD);
  }else{
    log_sensor->SetSensitiveDetector(bodySD);
  }
  
  G4UserLimits* stepLimit0;
  stepLimit0 = new  G4UserLimits(10*mm);// 1000*mm unpol 100*mm pol
  //stepLimit0 = new  G4UserLimits(1*km);
  //G4UserLimits* stepLimit;
  //stepLimit = new  G4UserLimits(0.001*mm);// 1000*mm unpol 100*mm pol

  logicFrame-> SetUserLimits(stepLimit0);
  

  //                                        
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
  logicFrame->SetVisAttributes (G4VisAttributes::Invisible);

  // G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.5,0.5,0.5));//gray
  // logic_CenterW->SetVisAttributes(atb);}
  // G4VisAttributes* vvanecol= new G4VisAttributes(G4Colour(0.9,0.9,0.9));
  G4VisAttributes* sensorcol= new G4VisAttributes(G4Colour(0.8,0.8,0.8,1.));//190, 190, 190　
  G4VisAttributes* fpccol= new G4VisAttributes(G4Colour(0.82,0.41,0.12,1.));//210., 105., 30.
  G4VisAttributes* asiccol= new G4VisAttributes(G4Colour(1.,1.,0.,1.));//255., 255., 0.
  G4VisAttributes* subcol= new G4VisAttributes(G4Color(0.,0.4,0.,1.)); // 0, 139, 69
  G4VisAttributes* tubecol= new G4VisAttributes(G4Colour(0.4,0.4,0.4,1.)); //105, 105, 105
  G4VisAttributes* fpgacol= new G4VisAttributes(G4Colour(0.5,0.5,0.5,1.)); // 
  G4VisAttributes* sfpcol= new G4VisAttributes(G4Colour(0.7,0.7,0.7,1.));
  G4VisAttributes* framecol= new G4VisAttributes(G4Colour(0.2,0.2,0.2,1.));
  G4VisAttributes* windowcol= new G4VisAttributes(G4Colour(0.82,0.41,0.12,1.));//210., 105., 30.
  // log_vvane->SetVisAttributes(vvanecol);
  log_vvaneUpper->SetVisAttributes (G4VisAttributes::Invisible);
  log_vvaneLower->SetVisAttributes (G4VisAttributes::Invisible);
  if(fDoStrip){
    log_sensor->SetVisAttributes(G4VisAttributes::Invisible);
    log_strip->SetVisAttributes(sensorcol);
    log_inactiveSensor->SetVisAttributes(sensorcol);
  }else{
    log_sensor->SetVisAttributes(sensorcol);
  }
  log_fpc->SetVisAttributes(fpccol);
  log_fpc2->SetVisAttributes(fpccol);
  log_sub->SetVisAttributes(subcol);
  log_sub2->SetVisAttributes(framecol);
  log_tube->SetVisAttributes(tubecol);
  log_asic->SetVisAttributes(asiccol);
  log_fpga->SetVisAttributes(fpgacol);
  log_sfp->SetVisAttributes(sfpcol);
  log_frame->SetVisAttributes(framecol);
  log_window->SetVisAttributes(windowcol);
  
  //
  //always return the physical World
  //

  return physiWorld;
}

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

