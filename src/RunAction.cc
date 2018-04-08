#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "ApplicationManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include <stdio.h>

//#include "TROOT.h"


RunAction::RunAction():theFileName("")
{
  runmessenger = new RunActionMessenger(this);

  // initialize root
  //  gROOT-> Reset();

}


RunAction::~RunAction()
{
}


void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  //initialize cumulative quantities
  //
  sumEAbs = sum2EAbs = 0.;
  sumLAbs = sum2LAbs = 0.; 

  //DataBroker 
  ApplicationManager* application = ApplicationManager::GetApplicationManager();
  application->Open(theFileName);
  application->Clear();
  ////DataBrokerEND///////////////
}


void RunAction::fillPerEvent(G4double EAbs,
			     G4double LAbs)
{
  //accumulate statistic
  //
  sumEAbs += EAbs;  sum2EAbs += EAbs*EAbs;
  sumLAbs += LAbs;  sum2LAbs += LAbs*LAbs;
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
  
  sumLAbs /= NbOfEvents; sum2LAbs /= NbOfEvents;
  G4double rmsLAbs = sum2LAbs - sumLAbs*sumLAbs;
  if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.;
  
  //print
  //
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean Energy in Absorber : " << G4BestUnit(sumEAbs,"Energy")
     << " +- "                          << G4BestUnit(rmsEAbs,"Energy")  
     << G4endl;
     
  G4cout
     << "\n mean trackLength in Absorber : " << G4BestUnit(sumLAbs,"Length")
     << " +- "                               << G4BestUnit(rmsLAbs,"Length")  
     << "\n------------------------------------------------------------\n"
     << G4endl;   

  //DataBroker 
  ApplicationManager* application = ApplicationManager::GetApplicationManager();

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

