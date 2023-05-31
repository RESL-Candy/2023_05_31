#include "SteppingAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (!fSiPMScoringVolume && !fPMTScoringVolume && !fOpticFiberScoringVolume) {
    const MyDetectorConstruction* detConstruction
      = static_cast<const MyDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    fSiPMScoringVolume = detConstruction->GetSiPMVolume();
    fPMTScoringVolume = detConstruction->GetPMTVolume();
    fOpticFiberScoringVolume = detConstruction->GetLLVolume();
  }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();


  // aStep->GetTrack()->GetCurrentStepNumber() == 0 
  if (aStep->GetTrack()->GetCurrentStepNumber() == 1 && aStep->GetTrack()->GetTrackID() == 1)
    //1번 스텝에 첫출발 입자를 잡기 
    {
      analysisManager->FillH1(5, aStep->GetPreStepPoint()->GetKineticEnergy());
      analysisManager->FillH1(6, aStep->GetPreStepPoint()->GetMomentum().z());
      analysisManager->FillH1(7, aStep->GetPreStepPoint()->GetPosition().z());
      //G4cout<<"첫빠따 출발이누"<<G4endl;
      //G4cout<<aStep->GetPreStepPoint()->GetKineticEnergy()<<G4endl;
      //G4cout<<aStep->GetTrack()->GetCurrentStepNumber() <<G4endl;
      
    }

  if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
    if(aStep->GetTrack()->GetCurrentStepNumber() == 1){
      fEventAction->AddCount_Whole(1);
      if(aStep->GetTrack()->GetParentID() == 1){
        fEventAction->AddCount_Positron(1);
      }
      if(aStep->GetTrack()->GetParentID() != 1){
        fEventAction->AddCount_notPositron(1);
      }
     // G4cout<<"띠용 "<<G4endl;
    }
  }
  // get volume of the current step
  G4LogicalVolume* volume
    = aStep->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fSiPMScoringVolume && volume != fPMTScoringVolume && volume != fOpticFiberScoringVolume){
    return;
    } 
  
  G4int copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetCopyNo();




  
  // get analysis manager
  




  
  // collect energy deposited in this step
  //G4double edepStep = step->GetTotalEnergyDeposit();
  //aStep->GetTrack()->GetParentID() == 1 /양전자 조건
  //aStep->GetTrack()->GetParentID() != 1 /양전자 아닌 조건
 /*
  if (volume == fSiPMScoringVolume){
    if(aStep->GetTrack()->GetParentID() == 1){
      if(copyNo==0){
	fEventAction->AddCount1(1);
	//G4cout<<"Score~! NO1"<<G4endl;
      }
      if(copyNo==1){
	fEventAction->AddCount2(1);
	//G4cout<<"Score~! NO2"<<G4endl;
      }
      if(copyNo==2){
	fEventAction->AddCount3(1);
	//G4cout<<"Score~! NO3"<<G4endl;
      }
      if(copyNo==3){
      fEventAction->AddCount4(1);
      //G4cout<<"Score~! NO4"<<G4endl;
      }
      if(copyNo==4){
	fEventAction->AddCount5(1);
	//G4cout<<"Score~! NO5"<<G4endl;
      }
      if(copyNo==5){
	fEventAction->AddCount6(1);
	//G4cout<<"Score~! NO6"<<G4endl;
      }
      if(copyNo==6){
	fEventAction->AddCount7(1);
	// G4cout<<"Score~! NO7"<<G4endl;
      }
      if(copyNo==7){
	fEventAction->AddCount8(1);
	// G4cout<<"Score~! NO8"<<G4endl;
      }
      if(copyNo==8){
	fEventAction->AddCount9(1);
	//G4cout<<"Score~! NO9"<<G4endl;
      }
      if(copyNo==9){
      fEventAction->AddCount10(1);
      //G4cout<<"Score~! NO10"<<G4endl;
      }
      if(copyNo==10){
	fEventAction->AddCount11(1);
	//G4cout<<"Score~! NO11"<<G4endl;
      }
      if(copyNo==11){
	fEventAction->AddCount12(1);
	// G4cout<<"Score~! NO12"<<G4endl;
      }
    }
    
   */ 

  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////SiPM scoring/////////////////////////////////////////
  //
  if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
    if (volume == fSiPMScoringVolume){
      if(aStep->GetTrack()->GetParentID() == 1){
    
      // fill ntuple
	analysisManager->FillH1(0, copyNo + 1);
	aStep->GetTrack()->SetTrackStatus(fStopAndKill);
      //analysisManager->FillNtupleDColumn(0,  copyNo + 1);
    
      //analysisManager->AddNtupleRow();
      }
    }
  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////PMT scoring//////////////////////////////////////////
  //
    if (volume == fPMTScoringVolume){
      analysisManager->FillH1(1, copyNo + 1);
      aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    
    }
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////OpticFiber Scoring///
    if (volume == fOpticFiberScoringVolume){
      if(aStep->GetTrack()->GetParentID() == 1){

	analysisManager->FillH1(0, copyNo + 1);
	aStep->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    }
  
  
  /*
    if (volume == fScoringVolume){
    G4cout<<"Score~!"<<G4endl;
    }
  */
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
