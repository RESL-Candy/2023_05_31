#ifndef CANDYEventAction_h
#define CANDYEventAction_h 1

#include "G4UserEventAction.hh"


#include "G4AnalysisManager.hh"


#include "RunAction.hh"
#include "G4UserRunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
/// Event action class
///




class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction* runAction);
  ~EventAction() override;
  
  void BeginOfEventAction(const G4Event* event) override;
  void EndOfEventAction(const G4Event* event) override;
  
  void AddCount_Whole(G4int Count1) { fCount1 += Count1; }
  void AddCount_Positron(G4int Count2) { fCount2 += Count2; }
  void AddCount_notPositron(G4int Count3) { fCount3 += Count3; }
  /*
  void AddCount1(G4int Count1) { fCount1 += Count1; }
  void AddCount2(G4int Count2) { fCount2 += Count2; }
  void AddCount3(G4int Count3) { fCount3 += Count3; }
  void AddCount4(G4int Count4) { fCount4 += Count4; }
  void AddCount5(G4int Count5) { fCount5 += Count5; }
  void AddCount6(G4int Count6) { fCount6 += Count6; }
  void AddCount7(G4int Count7) { fCount7 += Count7; }
  void AddCount8(G4int Count8) { fCount8 += Count8; }
  void AddCount9(G4int Count9) { fCount9 += Count9; }
  void AddCount10(G4int Count10) { fCount10 += Count10; }
  void AddCount11(G4int Count11) { fCount11 += Count11; }
  void AddCount12(G4int Count12) { fCount12 += Count12; }
  */
  
  
  
private:
  RunAction* fRunAction = nullptr;
  G4int   fCount1 = 0;
  G4int   fCount2 = 0;
  G4int   fCount3 = 0;
  /*
  G4int   fCount1 = 0;
  G4int   fCount2 = 0;
  G4int   fCount3 = 0;
  G4int   fCount4 = 0;
  G4int   fCount5 = 0;
  G4int   fCount6 = 0;
  G4int   fCount7 = 0;
  G4int   fCount8 = 0;
  G4int   fCount9 = 0;
  G4int   fCount10 = 0;
  G4int   fCount11 = 0;
  G4int   fCount12 = 0;
  */
  
  
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
