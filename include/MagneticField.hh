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
    void SetFieldFileName( const G4String val ){ fFieldFileName = val; }
    void SetKickerCalType( const G4String val ){ fKickerCalType = val; }
    void SetKickerFieldFileName( const G4String val ){ fKickerFieldFileName = val; }
    void SetWithSpin( const G4bool val ){ fWithSpin = val; }
    void FillFieldValue();
    void FillKickerFieldValue();

    void bflfit(double RM,double ZM,double &BR,double &BZ,double &APHI) const;
    void bfield(double CR,double CZ,double CI,double RI,double ZJ,double &BR,double &BZ,double &APHI) const;
    void cep12d(double RK,double &AK,double &AE) const;
    void kickSpatial(const G4double x[3], G4double &BRkick0, G4double &BYkick0) const;
    void kickMag(const G4double t, const G4double BRkick0, const G4double BYkick0, G4double &BRkick, G4double &BYkick) const;

private:
  static MagneticField* object;

  MagneticFieldMessenger* fMagFieldMessenger;
  G4String fCalType;
  G4String fFieldFileName;
  G4String fKickerCalType;
  G4String fKickerFieldFileName;
  G4bool   fWithSpin;

  // weak B-field parameters
  const G4double fXMU;
  G4int     fNF;
  G4double  fFLR[500],fFLZ[500],fFLCRNT[500];
  G4double  fFLRMin,fFLZMin;
  TGraph2D *fGraph_Bz;
  TGraph2D *fGraph_Br;

  // kicker B-field parameters
  const G4double fFacB;
  const G4double fStart_kick;
  const G4double fEndKickPar;
  const G4double fTK;
  const G4double fTau0;
  const G4double fTau;
  G4int    fKick_spatial_i;
  G4double fRatio;
  G4double fPosR[200],fPosY[200],fBYk[200],fBXk[200];
  G4double fEnd_kick;
};

#endif
