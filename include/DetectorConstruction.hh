#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "MagneticField.hh"
 
#include "G4Sphere.hh"

class G4Box;
class G4Tubs;
class G4Torus;
class G4Sphere;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:
     void SetMagField(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintCalorParameters(); 
                    
  private:

  G4Material*        sensorMat;
  G4Material*        subMat;
  G4Material*        tubeMat;
  G4Material*        fpcMat;
  G4Material*        frameMat;
  G4Material*        windowMat;
  
  G4Material*        defaultMaterial;
  G4Material*        defaultMaterial1;
  G4Material*        defaultMaterial2;
  G4Material*        defaultMaterial3;
  G4Material*        defaultMaterial4;
  G4Material*        defaultMaterial5;
  G4Material*        defaultMaterial6;
  G4Material*        defaultMaterial7;
  G4Material*        defaultMaterial8;
  G4double           WorldSize;

  
  G4Box*             solidWorld;    //pointer to the solid World 
  G4LogicalVolume*   logicWorld;    //pointer to the logical World
  G4VPhysicalVolume* physiWorld;    //pointer to the physical World

  G4Box*             solidFrame;    //pointer to the solid Frame 
  G4LogicalVolume*   logicFrame;    //pointer to the logicalFrame 
  G4VPhysicalVolume* physiFrame;    //pointer to the physicalFrame 
  
  G4Box*             sol_vvane;    
  G4LogicalVolume*   log_vvane;    
  G4VPhysicalVolume* phys_vvane; 
  
  G4Box*             sol_sensor;    
  G4LogicalVolume*   log_sensor;    
  G4VPhysicalVolume* phys_sensor; 
  G4VPhysicalVolume* phys_sensor1; 
  G4VPhysicalVolume* phys_sensor2; 
  G4VPhysicalVolume* phys_sensor3; 
  G4VPhysicalVolume* phys_sensor4; 
  
  G4Box*             sol_fpc;    
  G4LogicalVolume*   log_fpc;    
  G4VPhysicalVolume* phys_fpc; 
  
  G4Box*             sol_fpc2;    
  G4LogicalVolume*   log_fpc2;    
  G4VPhysicalVolume* phys_fpc2; 
  
  G4Box*             sol_asic;    
  G4LogicalVolume*   log_asic;    
  G4VPhysicalVolume* phys_asic; 
  
  G4Box*             sol_sub;    
  G4LogicalVolume*   log_sub;    
  G4VPhysicalVolume* phys_sub; 
  
  G4Box*             sol_sub2;    
  G4LogicalVolume*   log_sub2;    
  G4VPhysicalVolume* phys_sub2; 
  
  G4Tubs*             sol_tube;    
  G4LogicalVolume*   log_tube;    
  G4VPhysicalVolume* phys_tube; 

  G4Tubs*             sol_window;    
  G4LogicalVolume*   log_window;    
  G4VPhysicalVolume* phys_window; 
  
  G4Box*             sol_fpga;    
  G4LogicalVolume*   log_fpga;    
  G4VPhysicalVolume* phys_fpga; 

  G4Box*             sol_sfp;    
  G4LogicalVolume*   log_sfp;    
  G4VPhysicalVolume* phys_sfp; 

  //  G4Box*             sol_frame;    
  G4VSolid*          sol_frame;    
  G4LogicalVolume*   log_frame;    
  G4VPhysicalVolume* phys_frame; 
   
  

//////////////////////
     G4Tubs*             CenterW;    //pointer to the inner W 
     G4LogicalVolume*   logic_CenterW;    //pointer to the logical inner W 
     G4VPhysicalVolume* phys_CenterW;    //pointer to the physical inner W 
///////////
     G4Tubs*             CenterD;    //pointer to the inner D 
     G4LogicalVolume*   logic_CenterD;    //pointer to the logical inner D 
     G4VPhysicalVolume* phys_CenterD;    //pointer to the physical inner D 
///////////////
     G4Tubs*             CenterP;    //pointer to the inner P 
     G4LogicalVolume*   logic_CenterP;    //pointer to the logical inner P 
     G4VPhysicalVolume* phys_CenterP;    //pointer to the physical inner P 
/////
     G4Tubs*             CenterInP;    //pointer to the inner Pipe 
     G4LogicalVolume*   logic_CenterInP;    //pointer to the logical inner Pipe 
     G4VPhysicalVolume* phys_CenterInP;    //pointer to the physical inner Pipe 
     G4Tubs*             CenterOuP;    //pointer to the outer Pipe 
     G4LogicalVolume*   logic_CenterOuP;    //pointer to the logical outer Pipe 
     G4VPhysicalVolume* phys_CenterOuP;    //pointer to the physical outer Pipe 
////////////////////////////////////////////////
     G4Box*             panel1;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel1;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel1;    //pointer to the physical Calor

     G4Box*             panel2;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel2;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel2;    //pointer to the physical Calor

     G4Box*             panel3;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel3;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel3;    //pointer to the physical Calor

     G4Box*             panel4;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel4;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel4;    //pointer to the physical Calor

     G4Box*             panel5;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel5;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel5;    //pointer to the physical Calor

     G4Box*             panel6;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel6;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel6;    //pointer to the physical Calor

     G4Box*             panel7;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel7;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel7;    //pointer to the physical Calor

     G4Box*             panel8;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel8;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel8;    //pointer to the physical Calor
////////////////////////////////////////////////
     G4Box*             panel1a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel1a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel1a;    //pointer to the physical Calor

     G4Box*             panel2a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel2a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel2a;    //pointer to the physical Calor

     G4Box*             panel3a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel3a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel3a;    //pointer to the physical Calor

     G4Box*             panel4a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel4a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel4a;    //pointer to the physical Calor

     G4Box*             panel5a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel5a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel5a;    //pointer to the physical Calor

     G4Box*             panel6a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel6a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel6a;    //pointer to the physical Calor

     G4Box*             panel7a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel7a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel7a;    //pointer to the physical Calor

     G4Box*             panel8a;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel8a;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel8a;    //pointer to the physical Calor
////////////////////////////////////////////////
     G4Box*             panel1b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel1b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel1b;    //pointer to the physical Calor

     G4Box*             panel2b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel2b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel2b;    //pointer to the physical Calor

     G4Box*             panel3b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel3b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel3b;    //pointer to the physical Calor

     G4Box*             panel4b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel4b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel4b;    //pointer to the physical Calor

     G4Box*             panel5b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel5b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel5b;    //pointer to the physical Calor

     G4Box*             panel6b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel6b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel6b;    //pointer to the physical Calor

     G4Box*             panel7b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel7b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel7b;    //pointer to the physical Calor

     G4Box*             panel8b;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel8b;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel8b;    //pointer to the physical Calor
////////////////////////////////////////////////
     G4Box*             panel1c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel1c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel1c;    //pointer to the physical Calor

     G4Box*             panel2c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel2c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel2c;    //pointer to the physical Calor

     G4Box*             panel3c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel3c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel3c;    //pointer to the physical Calor

     G4Box*             panel4c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel4c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel4c;    //pointer to the physical Calor

     G4Box*             panel5c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel5c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel5c;    //pointer to the physical Calor

     G4Box*             panel6c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel6c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel6c;    //pointer to the physical Calor

     G4Box*             panel7c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel7c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel7c;    //pointer to the physical Calor

     G4Box*             panel8c;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel8c;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel8c;    //pointer to the physical Calor
//////////////////////////////////
////////////////////////////////////////////////
     G4Box*             panel1d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel1d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel1d;    //pointer to the physical Calor

     G4Box*             panel2d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel2d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel2d;    //pointer to the physical Calor

     G4Box*             panel3d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel3d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel3d;    //pointer to the physical Calor

     G4Box*             panel4d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel4d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel4d;    //pointer to the physical Calor

     G4Box*             panel5d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel5d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel5d;    //pointer to the physical Calor

     G4Box*             panel6d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel6d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel6d;    //pointer to the physical Calor

     G4Box*             panel7d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel7d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel7d;    //pointer to the physical Calor

     G4Box*             panel8d;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel8d;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel8d;    //pointer to the physical Calor
////////////////////////////////////////////////
     G4Box*             panel1e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel1e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel1e;    //pointer to the physical Calor

     G4Box*             panel2e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel2e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel2e;    //pointer to the physical Calor

     G4Box*             panel3e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel3e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel3e;    //pointer to the physical Calor

     G4Box*             panel4e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel4e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel4e;    //pointer to the physical Calor

     G4Box*             panel5e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel5e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel5e;    //pointer to the physical Calor

     G4Box*             panel6e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel6e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel6e;    //pointer to the physical Calor

     G4Box*             panel7e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel7e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel7e;    //pointer to the physical Calor

     G4Box*             panel8e;    //pointer to the solid Calor 
     G4LogicalVolume*   logic_panel8e;    //pointer to the logical Calor
     G4VPhysicalVolume* phys_panel8e;    //pointer to the physical Calor
//////////////////////////////////

     G4UniformMagField* magField;      //pointer to the magnetic field

  MagneticField* mymagField;
     
     DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
     void DefineMaterials();
     void ComputeCalorParameters();
     G4VPhysicalVolume* ConstructCalorimeter();     
};



inline void DetectorConstruction::ComputeCalorParameters()
{

}



#endif

