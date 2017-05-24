#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
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


DetectorConstruction::DetectorConstruction()
:defaultMaterial(0),
 magField(0), mymagField(0)
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


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}


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
G4Material* Al = new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.699*g/cm3);
G4Material* LAr = new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
//G4Material* PurePb = new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
G4Material* Fe = new G4Material("Fe", z= 26., a= 55.83*g/mole, density = 7.874*g/cm3);
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
 defaultMaterial  = Vacuum;
 sensorMat = PureSi;
 subMat = G10_Plate;
 tubeMat = LeadTangstate;
 //defaultMaterial1  = PurePb; 
 defaultMaterial1  = Al; 
 defaultMaterial2  = Air;
 defaultMaterial3  = WPb;
 defaultMaterial4  = PbWO4;
 defaultMaterial5  = PureSi;//Al;//Polycarbonate;
 defaultMaterial6  = LeadTangstate;
 defaultMaterial7  = Dyneema; 
 //defaultMaterial8  = PSF;
 defaultMaterial8  = Polymethylmethacrylate;
 //Al,Polycarbonate, Polymethylmethacrylate ,LeadTangstate
 //defaultMaterial3  = TPCGAS;
 //defaultMaterial5  = SiO2;
 fpcMat = Kapton;
 frameMat = CFRP;
 windowMat = Kapton;
}


G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
{

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the Calor parameters definition
  ComputeCalorParameters();
  
 
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
  //Another box
  //
  solidFrame = new G4Box("Frame",
                   0.5*WorldSize*0.95, 0.5*WorldSize*0.95, 0.5*WorldSize*0.95);
                       
  logicFrame = new G4LogicalVolume(solidFrame,		
                                   defaultMaterial,	
                                   "Frame");		
                                   
  physiFrame = new G4PVPlacement(0,			
  				 G4ThreeVector(),	
                                 logicFrame,		
                                 "Frame",		
                                 logicWorld,		
                                 false,			
                                 0);			


  G4double pi = TMath::Pi();
  G4int vane_num = 24;
  G4RotationMatrix* rotvu[vane_num];
  G4RotationMatrix* rotvd[vane_num];
  
  G4double d_center = 96.;
  G4double vvaneW = 217.5;
  G4double vvaneT = 6.;
  G4double vvaneH = 350.;
  G4double r_vane = 0.5*d_center + 0.5*vvaneW;
  
  G4double sensorW = 98.8;
  G4double sensorT = 0.32;
  G4double sensorH = 98.8;
  
  G4double subW2 = 17.;
  G4double deadspace = 0.25 * (vvaneW - subW2 - 2.*sensorW);
  G4double subH2 = 2.*sensorH + 4.*deadspace; 
  
  G4double subW = 217.5;
  G4double subT = 1.6;
  G4double subH = vvaneH - subH2; 
  
  G4double fpcW = 2.*(sensorW + deadspace);
  G4double fpcT = 0.1;
  G4double fpcH = subH2;

  G4double fpc2W = subW;
  G4double fpc2T = 0.125;
  G4double fpc2H = 80.;

  G4double asicW = 5;
  G4double asicT = 0.5;
  G4double asicH = 9.;

  G4double fpgaW = 40.;
  G4double fpgaT = 3.;
  G4double fpgaH = 40.;

  G4double sfpW = 10.;
  G4double sfpT = 10.;
  G4double sfpH = 70.;

  G4double frameW = subH2;
  G4double frameT = 4.;
  G4double frameH = subH2;


  //
  //virtual vane
  //
  sol_vvane = new G4Box("vvane", 0.5*vvaneW*mm, 0.5*vvaneT*mm, 0.5*vvaneH*mm);
  log_vvane = new G4LogicalVolume(sol_vvane,
				  defaultMaterial,
				  "vvane");
  
  for(int ii=0; ii<vane_num; ii++){
    rotvu[ii] = new G4RotationMatrix();
    G4double rangle = (G4double)2.*pi/vane_num*ii;
    rotvu[ii]->rotateZ(-rangle*rad);
    phys_vvane = new G4PVPlacement(rotvu[ii],
				   G4ThreeVector(r_vane*cos(rangle),
						 r_vane*sin(rangle),
						 vvaneH/2.),
				   log_vvane,
				   "vvane",
				   logicFrame,
				   false,
				   ii);
    rotvd[ii] = new G4RotationMatrix();
    
    rotvd[ii]->rotateX(pi*rad);
    rotvd[ii]->rotateZ(rangle*rad);
    phys_vvane = new G4PVPlacement(rotvd[ii],
				   G4ThreeVector(r_vane*cos(rangle),
						 r_vane*sin(rangle),
						 -vvaneH/2.),
				   log_vvane,
				   "vvane",
				   logicFrame,
				   false,
				   ii+vane_num);
  }

  
  //
  // --- silicon strip sensor
  //
  G4double xcenter = sensorW + deadspace + subW2 - 0.5*vvaneW; 
  G4double zcenter = sensorH + 2*deadspace - 0.5*vvaneH;   
  G4double splusd  = 0.5*sensorW + deadspace;

  sol_sensor = new G4Box("sensor", 0.5*sensorW*mm, 0.5*sensorT*mm, 0.5*sensorH*mm);
  log_sensor = new G4LogicalVolume(sol_sensor,
				   sensorMat,
				   "sensor");
  
  int ix=0; 
  int iy=0; 
  int iz=0; 
  for (int ii=0; ii<8; ii++){
    if (ii%2==0){ ix = 1;} else { ix = -1;} 
    if (ii/4==0){ iy = 1;} else { iy = -1;} 
    if ((ii/2)%2==0){ iz = 1;} else { iz = -1;} 
    
    //    std::cout << ii << " " << ix << " " << iy << " " << iz << std::endl;
    phys_sensor1 = new G4PVPlacement(0,
				     G4ThreeVector((xcenter + ix*splusd)*mm,
						   iy*0.5*frameT,
						   (zcenter + iz*splusd)*mm), 
				     log_sensor,
				     "sensor",
				     log_vvane,
				     false,
				     0);
  }


  //
  // -- FPC
  //
  sol_fpc = new G4Box("fpc", 0.5*fpcW*mm, 0.5*fpcT*mm, (0.5*fpcH-1.)*mm); // temporary
  log_fpc = new G4LogicalVolume(sol_fpc,
				fpcMat,
				"fpc");
  phys_fpc = new G4PVPlacement(0,
			       G4ThreeVector(xcenter*mm,
					     0.5*(frameT + sensorT + fpcT)*mm,
					     (zcenter+1.)*mm), // temporary 
			       log_fpc,
			       "fpc",
			       log_vvane,
			       false,
			       0);
  phys_fpc = new G4PVPlacement(0,
			       G4ThreeVector(xcenter*mm,
					     -0.5*(frameT + sensorT + fpcT)*mm,
					     (zcenter+1.)*mm), // temporary 
			       log_fpc,
			       "fpc",
			       log_vvane,
			       false,
			       0);
  
  sol_fpc2 = new G4Box("fpc2", 0.5*fpc2W*mm, 0.5*fpc2T*mm, 0.5*fpc2H*mm);
  log_fpc2 = new G4LogicalVolume(sol_fpc2,
				 fpcMat,
				 "fpc2");
  phys_fpc2 = new G4PVPlacement(0,
				G4ThreeVector(0.*mm,
					      0.5*(frameT + sensorT + fpcT)*mm,
					      (zcenter+sensorH+deadspace+0.5*fpc2H)*mm), 
				log_fpc2,
				"fpc2",
				log_vvane,
				false,
				0);
  phys_fpc2 = new G4PVPlacement(0,
				G4ThreeVector(0.*mm,
					      -0.5*(frameT + sensorT + fpcT)*mm,
					      (zcenter+sensorH+deadspace+0.5*fpc2H)*mm), 
				log_fpc2,
				"fpc2",
				log_vvane,
				false,
				0);

  //
  // --- Frame
  //
  G4VSolid *out = new G4Box("out", 0.5*frameW*mm, 0.5*frameT*mm, 0.5*frameH*mm);
  G4VSolid *in1 = new G4Box("in1", 93.8*0.5*mm, (0.5*frameT+0.5)*mm, 98.8*0.5*mm);
  G4VSolid *in2 = new G4Box("in2", 93.8*0.5*mm, (0.5*frameT+0.5)*mm, 98.8*0.5*mm);
  G4VSolid *in3 = new G4Box("in3", 93.8*0.5*mm, (0.5*frameT+0.5)*mm, 98.8*0.5*mm);
  G4VSolid *in4 = new G4Box("in4", 93.8*0.5*mm, (0.5*frameT+0.5)*mm, 98.8*0.5*mm);
  G4VSolid *tmp_frame1 = new G4SubtractionSolid("frame", out, in1, 0, G4ThreeVector(splusd*mm,0.,splusd*mm));
  G4VSolid *tmp_frame2 = new G4SubtractionSolid("frame", tmp_frame1, in2, 0, G4ThreeVector(splusd*mm,0.,-splusd*mm));
  G4VSolid *tmp_frame3 = new G4SubtractionSolid("frame", tmp_frame2, in3, 0, G4ThreeVector(-splusd*mm,0.,splusd*mm));
  sol_frame = new G4SubtractionSolid("frame", tmp_frame3, in4, 0, G4ThreeVector(-splusd*mm,0.,-splusd*mm));
  
  log_frame = new G4LogicalVolume(sol_frame,
				  frameMat, 
				  "frame");
  phys_frame = new G4PVPlacement(0,
				 G4ThreeVector(xcenter*mm,
					       0.*mm,
					       zcenter*mm),
				 log_frame,
				 "frame",
				 log_vvane,
				 false,
				 0);


  //
  // --- SliT128B, etc., just for fun
  //
  sol_asic = new G4Box("asic", 0.5*asicW*mm, 0.5*asicT*mm, 0.5*asicH*mm);
  log_asic = new G4LogicalVolume(sol_asic,
				 subMat, 
				 "asic");
  
  int jj=0;
  for (int ii=0; ii<32; ii++){
    jj = ii;
    if (ii > 15) jj = ii-16;
    phys_asic = new G4PVPlacement(0,
				  G4ThreeVector((-0.5*vvaneW + (jj+1)*12.)*mm,
						0.5*(frameT + sensorT + fpcT + asicT)*mm,
						(zcenter+sensorH+deadspace+0.5*fpc2H - 13.*(ii/16))*mm), 
				  log_asic,
				  "asic",
				  log_vvane,
				  false,
				  0);
  }
  for (int ii=0; ii<32; ii++){
    jj = ii;
    if (ii > 15) jj = ii-16;
    phys_asic = new G4PVPlacement(0,
				  G4ThreeVector((-0.5*vvaneW + (jj+1)*12.)*mm,
						-0.5*(frameT + sensorT + fpcT + asicT)*mm,
						(zcenter+sensorH+deadspace+0.5*fpc2H - 13.*(ii/16))*mm), 
				  log_asic,
				  "asic",
				  log_vvane,
				  false,
				  0);
  }
  
  
  sol_fpga = new G4Box("fpga", 0.5*fpgaW*mm, 0.5*fpgaT*mm, 0.5*fpgaH*mm);
  log_fpga = new G4LogicalVolume(sol_fpga,
				 subMat, 
				 "fpga");
  phys_fpga = new G4PVPlacement(0,
				G4ThreeVector(0.*mm,
					      0.5*(frameT + sensorT + fpcT + fpgaT)*mm,
					      (zcenter+sensorH+deadspace+fpc2H+35.)*mm), 
				log_fpga,
				"fpga",
				log_vvane,
				false,
				0);
  phys_fpga = new G4PVPlacement(0,
				G4ThreeVector(0.*mm,
					      -0.5*(frameT + sensorT + fpcT + fpgaT)*mm,
					      (zcenter+sensorH+deadspace+fpc2H+35.)*mm), 
				log_fpga,
				"fpga",
				log_vvane,
				false,
				0);

  sol_sfp = new G4Box("sfp", 0.5*sfpW*mm, 0.5*sfpT*mm, 0.5*sfpH*mm);
  log_sfp = new G4LogicalVolume(sol_sfp,
				subMat, 
				"sfp");
  phys_sfp = new G4PVPlacement(0,
			       G4ThreeVector(0.25*subW*mm,
					     0.5*(frameT + sensorT + fpcT + sfpT)*mm,
					     (zcenter+sensorH+deadspace+fpc2H+35.)*mm), 
			       log_sfp,
			       "sfp",
			       log_vvane,
			       false,
			       0);
  phys_sfp = new G4PVPlacement(0,
			       G4ThreeVector(0.25*subW*mm,
					     -0.5*(frameT + sensorT + fpcT + sfpT)*mm,
					     (zcenter+sensorH+deadspace+fpc2H+35.)*mm), 
			       log_sfp,
			       "sfp",
			       log_vvane,
			       false,
			       0);
  

  //
  // --- G10 sustrate
  //
  sol_sub = new G4Box("sub", subW/2*mm, subT/2*mm, subH/2*mm);
  log_sub = new G4LogicalVolume(sol_sub,
				subMat,
				"sub");
  phys_sub = new G4PVPlacement(0,
			       G4ThreeVector(-0.5*(vvaneW-subW)*mm,0,100*mm),
			       log_sub,
			       "sub",
			       log_vvane,
			       false,
			       0);

  sol_sub2 = new G4Box("sub2", subW2/2*mm, subT/2*mm, subH2/2*mm);
  log_sub2 = new G4LogicalVolume(sol_sub2,
				 subMat, // shoud be same as frame
				 "sub2");
  phys_sub2 = new G4PVPlacement(0,
				G4ThreeVector((xcenter - deadspace - sensorW - 0.5*subW2)*mm,0,zcenter*mm),
				log_sub2,
				"sub2",
				log_vvane,
				false,
				0);


  //
  // --- Center tube
  //
  G4double tube_din = 46.;
  G4double tube_dout = 96.;
  G4double tubeH = 700.;
  sol_tube = new G4Tubs("tube",tube_din/2*mm,tube_dout/2*mm,tubeH/2*mm,
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
  // --- polyimide window
  //
  G4double window_din = 218.*2.+tube_dout;
  G4double window_dout = 218.2*2.+tube_dout;
  G4double windowH = 200.;
  sol_window = new G4Tubs("window",window_din/2*mm,window_dout/2*mm,windowH/2*mm,
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
  log_sensor->SetSensitiveDetector(bodySD);


  G4UserLimits* stepLimit0;
  stepLimit0 = new  G4UserLimits(10*mm);// 1000*mm unpol 100*mm pol
  G4UserLimits* stepLimit;
  stepLimit = new  G4UserLimits(0.001*mm);// 1000*mm unpol 100*mm pol

  
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
  log_vvane->SetVisAttributes (G4VisAttributes::Invisible);
  log_sensor->SetVisAttributes(sensorcol);
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


void DetectorConstruction::PrintCalorParameters()
{}


#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "MagneticField.hh"

void DetectorConstruction::SetMagField(G4double fieldValue)
{

  //apply a global uniform magnetic field along Z axis

  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();


  mymagField = new MagneticField();

  mymagField->pStepper = new G4ClassicalRK4(mymagField->fEquation,12);

  fieldMgr->SetDetectorField(mymagField);
  fieldMgr->CreateChordFinder(mymagField);


}


#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}

