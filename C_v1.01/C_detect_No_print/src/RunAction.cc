#include "RunAction.hh"






RunAction::RunAction()
{
  // add new units for dose
  //
  /*
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);
  */

  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  // analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output


  // Creating ntuple
  //
  analysisManager->CreateH1("SiPMScoring","SiPMscoring", 12, 0.5, 12.5);
  analysisManager->CreateH1("PMTScoring","PMTscoring", 2, 0.5, 2.5);
  analysisManager->CreateH1("Whole Scoring","Whole scoring", 100000, 0.5, 100000.5);
  analysisManager->CreateH1("Positron Scoring","Positron scoring", 48000, 0.5, 48000.5);
  analysisManager->CreateH1("non P Scoring","non P scoring", 48000, 0.5, 48000.5);
  analysisManager->CreateH1("Positron Energy Scoring","Positron Energy scoring", 200, 0., 4.);
  analysisManager->CreateH1("Positron z_momentum Scoring","Positron z_momentum scoring", 200, -10., 10.);
  analysisManager->CreateH1("Positron z_position Scoring","Positron z_position scoring", 120, -10., 10.);

  
  // analysisManager->CreateNtuple("CANDY_tree", "Scoring");
  // analysisManager->CreateNtupleDColumn("Score");

  // analysisManager->FinishNtuple();



  /*
  //Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fCount1);
  accumulableManager->RegisterAccumulable(fCount2);
  accumulableManager->RegisterAccumulable(fCount3);
  accumulableManager->RegisterAccumulable(fCount4);
  accumulableManager->RegisterAccumulable(fCount5);
  accumulableManager->RegisterAccumulable(fCount6);
  accumulableManager->RegisterAccumulable(fCount7);
  accumulableManager->RegisterAccumulable(fCount8);
  accumulableManager->RegisterAccumulable(fCount9);
  accumulableManager->RegisterAccumulable(fCount10);
  accumulableManager->RegisterAccumulable(fCount11);
  accumulableManager->RegisterAccumulable(fCount12);
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* )
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  /*
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  */  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "Candy.root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  // G4String fileName = "B4.hdf5";
  // G4String fileName = "B4.xml";
  analysisManager->OpenFile(fileName);
  G4cout << "Open File " << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  /*
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  
  G4int count1  = fCount1.GetValue();
  G4int count2  = fCount2.GetValue();
  G4int count3  = fCount3.GetValue();
  G4int count4  = fCount4.GetValue();
  G4int count5  = fCount5.GetValue();
  G4int count6  = fCount6.GetValue();
  G4int count7  = fCount7.GetValue();
  G4int count8  = fCount8.GetValue();
  G4int count9  = fCount9.GetValue();
  G4int count10  = fCount10.GetValue();
  G4int count11  = fCount11.GetValue();
  G4int count12  = fCount12.GetValue();
  
  const MyDetectorConstruction* detConstruction
   = static_cast<const MyDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  
  
  G4double mass = detConstruction->GetSiPMVolume()->GetMass();
*/
  // Run conditions

  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  
  
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
/*
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << "Co1 : "<<count1
     << G4endl
     << "Co2 : "<<count2
     << G4endl
     << "Co3 : "<<count3
     << G4endl
     << "Co4 : "<<count4
     << G4endl
     << "Co5 : "<<count5
     << G4endl
     << "Co6 : "<<count6
     << G4endl
     << "Co7 : "<<count7
     << G4endl
     << "Co8 : "<<count8
     << G4endl
     << "Co9 : "<<count9
     << G4endl
     << "Co10 : "<<count10
     << G4endl
     << "Co11 : "<<count11
     << G4endl
     << "Co12 : "<<count12
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
*/
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  // G4cout<<"루트파일 만들어짐"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void RunAction::AddCount1(G4int Count1)
{
  fCount1  += Count1;
}
void RunAction::AddCount2(G4int Count2)
{
  fCount2  += Count2;
}
void RunAction::AddCount3(G4int Count3)
{
  fCount3  += Count3;
}
void RunAction::AddCount4(G4int Count4)
{
  fCount4  += Count4;
}
void RunAction::AddCount5(G4int Count5)
{
  fCount5  += Count5;
}
void RunAction::AddCount6(G4int Count6)
{
  fCount6  += Count6;
}
void RunAction::AddCount7(G4int Count7)
{
  fCount7  += Count7;
}
void RunAction::AddCount8(G4int Count8)
{
  fCount8  += Count8;
}
void RunAction::AddCount9(G4int Count9)
{
  fCount9  += Count9;
}
void RunAction::AddCount10(G4int Count10)
{
  fCount10  += Count10;
}
void RunAction::AddCount11(G4int Count11)
{
  fCount11  += Count11;
}
void RunAction::AddCount12(G4int Count12)
{
  fCount12  += Count12;
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::printEventproc()
{
  

  // nofEvents=1000;
  // G4int div =20;
  /*
  G4RunManager* runManager = G4RunManager::GetRunManager();
  G4int numEvents = runManager->GetCurrentRun()->GetNumberOfEvent();
  G4int eventID = runManager->GetCurrentEvent()->GetEventID();
  
  G4double progressPercent = static_cast<G4double>(eventID) / static_cast<G4double>(numEvents) * 100.0;

    G4cout << "Progress: " << progressPercent << "%" << G4endl;
    
  if(fEvent % (nofEvents/div) == 0)
    {
      G4double percent = fEvent; 
      G4cout<<"진행률 : "<<percent/nofEvents * 100  <<"%"<<G4endl;
    }
    */


    fEvent += 1;
  if(fEvent % 200 == 0)
    {
      G4cout<<"처리 Events : "<<fEvent<<"개"<<G4endl;
    }
  
}
