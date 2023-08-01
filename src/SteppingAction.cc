#include "SteppingAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// collect energy deposited in this step
//G4double edepStep = step->GetTotalEnergyDeposit();
//aStep->GetTrack()->GetParentID() == 1 /양전자 조건
//aStep->GetTrack()->GetParentID() != 1 /양전자 아닌 조건


void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (!fSiPMScoringVolume )
    {
      const MyDetectorConstruction* detConstruction = static_cast<const MyDetectorConstruction*>
	(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
      
      fSiPMScoringVolume = detConstruction->GetSiPMVolume();
    }
  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  

  // aStep->GetTrack()->GetCurrentStepNumber() == 0 
  if (aStep->GetTrack()->GetCurrentStepNumber() == 1 && aStep->GetTrack()->GetTrackID() == 1)
    //1번 스텝에 첫출발 입자를 잡기 
    {
      // fill ntuple
      analysisManager->FillNtupleDColumn(1, aStep->GetPreStepPoint()->GetKineticEnergy());

    }

  if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton")
    {
      if(aStep->GetTrack()->GetCurrentStepNumber() == 1)
        {
          fEventAction->AddCount_Whole(1);
        } 
    }
  // get volume of the current step
  G4LogicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle() ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fSiPMScoringVolume ){
    return;
  } 




  G4int copyNo = 0;

  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////SiPM scoring/////////////////////////////////////////
  //
  if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton")
    {
      if (volume == fSiPMScoringVolume)
	{
	  copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();  
	  // fill ntuple
	  fEventAction->AddCount_SiPM(fEventAction->f_SiPM_Count, copyNo);
	  fEventAction->AddCount_WholeSiPM(1);
	  //analysisManager->FillNtupleDColumn(copyNo+12,  1);
	  aStep->GetTrack()->SetTrackStatus(fStopAndKill);        
	}
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
