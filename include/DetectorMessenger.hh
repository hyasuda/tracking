#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
//class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
//class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();
    
  void SetNewValue(G4UIcommand*,G4String newValue);
    
  private:
    DetectorConstruction* Detector;
    
    G4UIdirectory*             muDir;
    G4UIdirectory*             detDir;
  //G4UIcmdWithAString*        AbsMaterCmd;
  //G4UIcmdWithAString*        GapMaterCmd;
  //G4UIcmdWithADoubleAndUnit* AbsThickCmd;
  //G4UIcmdWithADoubleAndUnit* GapThickCmd;
  //G4UIcmdWithADoubleAndUnit* SizeYZCmd;
    G4UIcmdWithAnInteger*      NbVanesCmd;    
    G4UIcmdWithoutParameter*   UpdateCmd;
    G4UIcmdWithABool*          fDoStripCmd;
};

#endif

