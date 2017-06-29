// $Id: MagneticField.hh,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#ifndef MAGNETICFIELD_HH
#define MAGNETICFIELD_HH

#include "globals.hh"
#include "G4MagneticField.hh"
//#include "G4Mag_SpinEqRhs.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

class MagneticField
  : public G4MagneticField
{

  private:

  public:
  //G4Mag_SpinEqRhs* fEquation;
  G4ChordFinder* fChordFinder;
  G4MagIntegratorStepper* pStepper;
  G4EqEMFieldWithSpin* fEquation;
  G4PropagatorInField*    fieldPropagator;

  void updateField();

  public:

  static MagneticField* getObject();

    MagneticField();
    ~MagneticField();

  public:
    void GetFieldValue( const G4double Point[3],  G4double* Bfield ) const;

private:
  static MagneticField* object;

};

#endif
