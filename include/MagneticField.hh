// $Id: MagneticField.hh,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#ifndef MAGNETICFIELD_HH
#define MAGNETICFIELD_HH

#include "globals.hh"
#include "G4MagneticField.hh"
//#include "G4Mag_SpinEqRhs.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

#include "TGraph2D.h"

class MagneticField
  : public G4MagneticField
{

  private:

  public:
  //G4Mag_SpinEqRhs* fEquation;
  G4ChordFinder* fChordFinder;
  G4MagIntegratorStepper* pStepper;
  //G4EqEMFieldWithSpin* fEquation;
  G4EqMagElectricField* fEquation;
  G4PropagatorInField*    fieldPropagator;

  void updateField();

  public:

  static MagneticField* getObject();

    MagneticField();
    ~MagneticField();

  public:
    void GetFieldValue( const G4double Point[4],  G4double* Bfield ) const;

    void bflfit(int NF,double FLR[500],double FLZ[500],double FLCRNT[500],double RM,double ZM,double &BR,double &BZ,double &APHI);
    void bfield(double CR,double CZ,double CI,double RI,double ZJ,double &BR,double &BZ,double &APHI) const;
    void cep12d(double RK,/*double I,*/double &AK,double &AE,double &ILL) const;

private:
  static MagneticField* object;

  G4int fNF;
  G4double fFLR[500],fFLZ[500],fFLCRNT[500];
  TGraph2D *fGraph_Bz;
  TGraph2D *fGraph_Br;

};

#endif
