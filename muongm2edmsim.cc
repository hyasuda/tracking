#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "Randomize.hh"

//#include "QGSP.hh"
#include "DetectorConstruction.hh"
#include "F04PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "SteppingVerbose.hh"

#define G4VIS_USE

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4UI_USE_WIN32
#include "G4UIWin32.hh"
#endif


#include "G4GDMLParser.hh"
#include "G4TransportationManager.hh"

/*
#ifdef G4UI_USE_QT
#include "G4UIQt.hh"
#include "G4Qt.hh"
#include <qapplication.h>
#endif
*/


G4int DokodemoBango; 
G4int PickBegin; 


int main(int argc,char** argv)
{

 DokodemoBango=1; 
  // Choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // User Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
     
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);

  //  G4VUserPhysicsList* physics = new QGSP;
  //G4VUserPhysicsList* physics = new ExN03PhysicsList;
  G4VUserPhysicsList* physics = new F04PhysicsList("QGSP_BERT");//Toshito-san
  runManager->SetUserInitialization(physics);

    
  // Set user action classes
  G4VUserPrimaryGeneratorAction* gen_action = 
                          new PrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);
  RunAction* run_action = new RunAction;  
  runManager->SetUserAction(run_action);

  EventAction* event_action = new EventAction(run_action);
  runManager->SetUserAction(event_action);

  G4UserSteppingAction* stepping_action =
                    new SteppingAction(detector, event_action);
  runManager->SetUserAction(stepping_action);


  // Initialize G4 kernel
  //
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();      
  
  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);    
      //
      //tyosioka 170421 for GDML
      //
      G4GDMLParser parser;
      if (argc>=3){
	parser.Write(argv[2], G4TransportationManager::GetTransportationManager()->
		     GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume());
      }
    }
  else           // interactive mode : define visualization UI terminal
    {
#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
#endif

      G4UIsession* session = 0;
#if defined(G4UI_USE_TCSH)
      session = new G4UIterminal(new G4UItcsh);      
#elif defined(G4UI_USE_XM)
      session = new G4UIXm(argc,argv);
      UI->ApplyCommand("/control/execute visTutor/gui.mac");      
#elif defined(G4UI_USE_WIN32)
      session = new G4UIWin32();
      UI->ApplyCommand("/control/execute visTutor/gui.mac");      
#elif defined(G4UI_USE_QT)
      session = new G4UIQt(argc,argv);
      UI->ApplyCommand("/control/execute visTutor/gui.mac");      
#else
      session = new G4UIterminal();
#endif

      //      UI->ApplyCommand("/control/execute vis3.mac");
      UI->ApplyCommand("/control/execute originvis.mac");
      session->SessionStart();
      delete session;
      
#ifdef G4VIS_USE
      delete visManager;
#endif                
    }

  delete runManager;

  return 0;
}


