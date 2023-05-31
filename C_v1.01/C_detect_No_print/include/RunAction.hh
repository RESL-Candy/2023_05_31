
#ifndef CANDYRunAction_h
#define CANDYRunAction_h 1


#include "PrimaryGeneratorAction.hh"
#include "Construction.hh"


#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"

#include "G4AnalysisManager.hh" //for make root file

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"



//#include "globals.hh"
//class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.


class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction() override;
  
  void BeginOfRunAction(const G4Run*) override;
  void   EndOfRunAction(const G4Run*) override;
  void printEventproc();
/*  
  void AddCount1 (G4int Count1);
  void AddCount2 (G4int Count2);
  void AddCount3 (G4int Count3);
  void AddCount4 (G4int Count4);
  void AddCount5 (G4int Count5);
  void AddCount6 (G4int Count6);
  void AddCount7 (G4int Count7);
  void AddCount8 (G4int Count8);
  void AddCount9 (G4int Count9);
  void AddCount10 (G4int Count10);
  void AddCount11 (G4int Count11);
  void AddCount12 (G4int Count12);

  


  G4Accumulable<G4int> fCount1 = 0;
  G4Accumulable<G4int> fCount2 = 0;
  G4Accumulable<G4int> fCount3 = 0;
  G4Accumulable<G4int> fCount4 = 0;
  G4Accumulable<G4int> fCount5 = 0;
  G4Accumulable<G4int> fCount6 = 0;
  G4Accumulable<G4int> fCount7 = 0;
  G4Accumulable<G4int> fCount8 = 0;
  G4Accumulable<G4int> fCount9 = 0;
  G4Accumulable<G4int> fCount10 = 0;
  G4Accumulable<G4int> fCount11 = 0;
  G4Accumulable<G4int> fCount12 = 0;
  
  
*/

private:
  G4int fEvent =0;
  G4int nofEvents=0;  
};


#endif

