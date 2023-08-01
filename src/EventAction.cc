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

  for(int i=0;i<40;i++){
    f_SiPM_Count[i] = {0,};
  
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  fEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, fEventID);

  for(G4int i=0; i<40; i++){
    analysisManager->FillNtupleDColumn(i+2, f_SiPM_Count[i]);
  }
  analysisManager->FillNtupleDColumn(42,fCount1);
  analysisManager->FillNtupleDColumn(43,fCount2);
  analysisManager->AddNtupleRow();  
  fRunAction->printEventproc();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

