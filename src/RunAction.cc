#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "ApplicationManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include <stdio.h>

#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"


RunAction::RunAction():theFileName("")
{
  runmessenger = new RunActionMessenger(this);

  // initialize root
  //  gROOT-> Reset();
  // define histograms
  hist_shower = new TH1D("shower", "Shower Development", 2000, 0., 2000.);
  hist_shower-> GetXaxis()-> SetTitle("Depth");
  hist_shower-> GetYaxis()-> SetTitle("Deposit Energy (MeV)");
 }


RunAction::~RunAction()
{
   delete hist_shower;
   printf("hist_shower delete\n");
}


void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  //initialize cumulative quantities
  //
  sumEAbs = sum2EAbs =sumEGap = sum2EGap = 0.;
  sumLAbs = sum2LAbs =sumLGap = sum2LGap = 0.; 

//DataBroker 
  ApplicationManager* application =
    ApplicationManager::GetApplicationManager();
  application -> SetEdepByRun( 0.0 );
  application->Open(theFileName);
  application->Clear();
////DataBrokerEND///////////////

  hist_shower->Reset();
  printf("hist_shower Reset\n");
}


void RunAction::fillPerEvent(G4double EAbs, G4double EGap,
			     G4double LAbs, G4double LGap)
{
  //accumulate statistic
  //
  sumEAbs += EAbs;  sum2EAbs += EAbs*EAbs;
  sumEGap += EGap;  sum2EGap += EGap*EGap;
  
  sumLAbs += LAbs;  sum2LAbs += LAbs*LAbs;
  sumLGap += LGap;  sum2LGap += LGap*LGap;  
}


void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  //compute statistics: mean and rms
  //
  sumEAbs /= NbOfEvents; sum2EAbs /= NbOfEvents;
  G4double rmsEAbs = sum2EAbs - sumEAbs*sumEAbs;
  if (rmsEAbs >0.) rmsEAbs = std::sqrt(rmsEAbs); else rmsEAbs = 0.;
  
  sumEGap /= NbOfEvents; sum2EGap /= NbOfEvents;
  G4double rmsEGap = sum2EGap - sumEGap*sumEGap;
  if (rmsEGap >0.) rmsEGap = std::sqrt(rmsEGap); else rmsEGap = 0.;
  
  sumLAbs /= NbOfEvents; sum2LAbs /= NbOfEvents;
  G4double rmsLAbs = sum2LAbs - sumLAbs*sumLAbs;
  if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.;
  
  sumLGap /= NbOfEvents; sum2LGap /= NbOfEvents;
  G4double rmsLGap = sum2LGap - sumLGap*sumLGap;
  if (rmsLGap >0.) rmsLGap = std::sqrt(rmsLGap); else rmsLGap = 0.;
  
  //print
  //
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean Energy in Absorber : " << G4BestUnit(sumEAbs,"Energy")
     << " +- "                          << G4BestUnit(rmsEAbs,"Energy")  
     << "\n mean Energy in Gap      : " << G4BestUnit(sumEGap,"Energy")
     << " +- "                          << G4BestUnit(rmsEGap,"Energy")
     << G4endl;
     
  G4cout
     << "\n mean trackLength in Absorber : " << G4BestUnit(sumLAbs,"Length")
     << " +- "                               << G4BestUnit(rmsLAbs,"Length")  
     << "\n mean trackLength in Gap      : " << G4BestUnit(sumLGap,"Length")
     << " +- "                               << G4BestUnit(rmsLGap,"Length")
     << "\n------------------------------------------------------------\n"
     << G4endl;   

//DataBroker 
  ApplicationManager* application =
    ApplicationManager::GetApplicationManager();

  G4double total = application -> GetEdepByRun();

  G4cout << "----" << G4endl;
  G4cout << "TOTAL:" << total/MeV << "MeV" << G4endl;

  std::ofstream& ofs = application -> GetFileStream();
  ofs.close();

  application->Update();
  application->Save();
/////DataBrokerEND///////////////

}


G4String RunAction::AsString( G4int runid, G4int digit ) const
{
  char temp[33];
  sprintf( temp, "%d", digit );

  G4String format = "%.";
  format += temp;
  format += "d";

  char* idstr = new char[digit+1];
  sprintf( idstr, format.c_str(), runid );

  G4String retval = idstr;
  delete [] idstr;

  return retval;
}

