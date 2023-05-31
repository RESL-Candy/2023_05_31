#include "EventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  fCount1 = 0;
  fCount2 = 0;
  fCount3 = 0;
  /*
  fCount1 = 0;
  fCount2 = 0;
  fCount3 = 0;
  fCount4 = 0;
  fCount5 = 0;
  fCount6 = 0;
  fCount7 = 0;
  fCount8 = 0;
  fCount9 = 0;
  fCount10 = 0;
  fCount11 = 0;
  fCount12 = 0;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(2, fCount1);
  analysisManager->FillH1(3, fCount2);
  analysisManager->FillH1(4, fCount3);
  fRunAction->printEventproc();
  /*
  // accumulate statistics in run action
  fRunAction->AddCount1(fCount1);
  fRunAction->AddCount2(fCount2);
  fRunAction->AddCount3(fCount3);
  fRunAction->AddCount4(fCount4);
  fRunAction->AddCount5(fCount5);
  fRunAction->AddCount6(fCount6);
  fRunAction->AddCount7(fCount7);
  fRunAction->AddCount8(fCount8);
  fRunAction->AddCount9(fCount9);
  fRunAction->AddCount10(fCount10);
  fRunAction->AddCount11(fCount11);
  fRunAction->AddCount12(fCount12);
  fRunAction->printEventproc();
  */  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

