// $Id: MagneticField.hh,v 1.1.1.1 2004/10/06 05:36:32 iwai Exp $
#ifndef MAGNETICFIELD_HH
#define MAGNETICFIELD_HH

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

#include "TGraph2D.h"

class MagneticFieldMessenger;

class MagneticField
  : public G4MagneticField
{

  private:

  public:
  G4ChordFinder*          fChordFinder;
  G4MagIntegratorStepper* pStepper;
  G4EqMagElectricField*   fEquation;
  G4EqEMFieldWithSpin*    fEquationSpin;
  G4PropagatorInField*    fieldPropagator;

  void updateField();

  public:

  static MagneticField* getObject();

    MagneticField();
    ~MagneticField();

  public:
    void GetFieldValue( const G4double Point[4],  G4double* Bfield ) const;
    void SetCalType( const G4String val ){ fCalType = val; }
    void SetWithSpin( const G4bool val ){ fWithSpin = val; }
    void FillFieldValue();

    void bflfit(int NF,double FLR[500],double FLZ[500],double FLCRNT[500],double RM,double ZM,double &BR,double &BZ,double &APHI);
    void bfield(double CR,double CZ,double CI,double RI,double ZJ,double &BR,double &BZ,double &APHI) const;
    void cep12d(double RK,/*double I,*/double &AK,double &AE,double &ILL) const;

private:
  static MagneticField* object;

  MagneticFieldMessenger* fMagFieldMessenger;
  G4String fCalType;
  G4bool   fWithSpin;

  G4int fNF;
  G4double fFLR[500],fFLZ[500],fFLCRNT[500];
  TGraph2D *fGraph_Bz;
  TGraph2D *fGraph_Br;

};

#endif
