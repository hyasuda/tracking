#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
  DetectorConstruction(const G4int nvane=40,const G4bool doStrip=true);
   ~DetectorConstruction();

  public:
    G4bool SetNumberOfVanes(G4int nvane){if(nvane!=fNvane){fNvane=nvane;return true;}else{return false;}}
    G4bool SetDoStrip(G4bool doStrip){if(doStrip!=fDoStrip){fDoStrip=doStrip;return true;}else{return false;}}
    G4VPhysicalVolume* Construct();

    void UpdateGeometry();
      
  private:

  G4Material*        sensorMat;
  G4Material*        subMat;
  G4Material*        tubeMat;
  G4Material*        shieldMat;
  G4Material*        fpcMat;
  G4Material*        frameMat;
  G4Material*        windowMat;
  
  G4Material*        defaultMaterial;
  G4Material*        storageGas;

  G4double           WorldSize;
  G4int              fNvane;
  G4bool             fDoStrip;

  G4Box*             solidWorld;    //pointer to the solid World 
  G4LogicalVolume*   logicWorld;    //pointer to the logical World
  G4VPhysicalVolume* physiWorld;    //pointer to the physical World

  G4Box*             solidFrame;    //pointer to the solid Frame 
  G4LogicalVolume*   logicFrame;    //pointer to the logicalFrame 
  G4VPhysicalVolume* physiFrame;    //pointer to the physicalFrame 
  
  G4Box*             sol_vvane;    
  G4LogicalVolume*   log_vvaneUpper;
  G4LogicalVolume*   log_vvaneLower;
  G4VPhysicalVolume* phys_vvaneUpper;
  G4VPhysicalVolume* phys_vvaneLower;

  G4Box*             sol_sensor;    
  G4LogicalVolume*   log_sensor; 
  G4VPhysicalVolume* phys_sensor; 
  
  G4Box*             sol_strip;
  G4LogicalVolume*   log_strip;
  G4VPhysicalVolume* phys_strip;

  G4VSolid*          sol_inactiveSensor;
  G4LogicalVolume*   log_inactiveSensor;
  G4VPhysicalVolume* phys_inactiveSensor;

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
  
  G4Tubs*            sol_tube;    
  G4LogicalVolume*   log_tube;    
  G4VPhysicalVolume* phys_tube; 

  G4Tubs*            sol_window;    
  G4LogicalVolume*   log_window;    
  G4VPhysicalVolume* phys_window; 
  
  G4Box*             sol_fpga;    
  G4LogicalVolume*   log_fpga;    
  G4VPhysicalVolume* phys_fpga; 

  G4Box*             sol_sfp;    
  G4LogicalVolume*   log_sfp;    
  G4VPhysicalVolume* phys_sfp; 

  G4VSolid*          sol_frame;    
  G4LogicalVolume*   log_frame;    
  G4VPhysicalVolume* phys_frame; 

  G4Tubs*            sol_shield;
  G4LogicalVolume*   log_shield;
  G4VPhysicalVolume* phys_shield;

  DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
  void DefineMaterials();
};

#endif

