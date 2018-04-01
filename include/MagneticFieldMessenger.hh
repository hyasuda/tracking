#ifndef MagneticFieldMessenger_h
#define MagneticFieldMessenger_h

#include "globals.hh"
#include "G4UImessenger.hh"

class MagneticField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class MagneticFieldMessenger: public G4UImessenger
{
public:
  MagneticFieldMessenger(MagneticField* );
  ~MagneticFieldMessenger();
    
  void SetNewValue(G4UIcommand*,G4String newValue);
    
  private:
    MagneticField*             fMag;
    
    G4UIdirectory*             fMagDir;
    G4UIcmdWithAString*        fCalTypeCmd;
    G4UIcmdWithAString*        fFieldFileCmd;
    G4UIcmdWithAString*        fKickerCalTypeCmd;
    G4UIcmdWithAString*        fKickerFieldFileCmd;
    G4UIcmdWithABool*          fWithSpinCmd;
};

#endif

