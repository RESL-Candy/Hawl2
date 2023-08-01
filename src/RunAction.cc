#include "RunAction.hh"






RunAction::RunAction()
{
  
  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("Candy");
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output


  // Creating ntuple
  //
  //Creating Histo
  /*
  analysisManager->CreateH1("SiPMScoring","SiPMscoring", 12, 0.5, 12.5);
  analysisManager->CreateH1("PMTScoring","PMTscoring", 2, 0.5, 2.5);
  analysisManager->CreateH1("Whole Scoring","Whole scoring", 100001, -0.5, 100000.5);
  analysisManager->CreateH1("Positron Scoring","Positron scoring", 48001, -0.5, 48000.5);
  analysisManager->CreateH1("non P Scoring","non P scoring", 48001, -0.5, 48000.5);
  analysisManager->CreateH1("Positron Energy Scoring","Positron Energy scoring", 200, 0., 4.);
  analysisManager->CreateH1("Positron z_momentum Scoring","Positron z_momentum scoring", 200, -10., 10.);
  analysisManager->CreateH1("Positron z_position Scoring","Positron z_position scoring", 230, -11.5, 11.5);
  */

  
  analysisManager->CreateNtuple("HAWL", "Event_by_Event");
  analysisManager->CreateNtupleDColumn("Event_ID");
  
  analysisManager->CreateNtupleDColumn("Muon_Energy");

  analysisManager->CreateNtupleDColumn("delta_X");
  analysisManager->CreateNtupleDColumn("delta_Y");
  analysisManager->CreateNtupleDColumn("delta_R");
  
  //SiPM Scoring
  analysisManager->CreateNtupleDColumn("1_SiPM");//0
  analysisManager->CreateNtupleDColumn("2_SiPM");//1
  analysisManager->CreateNtupleDColumn("3_SiPM");//2
  analysisManager->CreateNtupleDColumn("4_SiPM");//3
  analysisManager->CreateNtupleDColumn("5_SiPM");//4
  analysisManager->CreateNtupleDColumn("6_SiPM");//5
  analysisManager->CreateNtupleDColumn("7_SiPM");//6
  analysisManager->CreateNtupleDColumn("8_SiPM");//7
  analysisManager->CreateNtupleDColumn("9_SiPM");//8
  analysisManager->CreateNtupleDColumn("10_SiPM");//9
  analysisManager->CreateNtupleDColumn("11_SiPM");//10
  analysisManager->CreateNtupleDColumn("12_SiPM");//11
  analysisManager->CreateNtupleDColumn("13_SiPM");//12
  analysisManager->CreateNtupleDColumn("14_SiPM");//13
  analysisManager->CreateNtupleDColumn("15_SiPM");//14
  analysisManager->CreateNtupleDColumn("16_SiPM");//15
  analysisManager->CreateNtupleDColumn("17_SiPM");//16
  analysisManager->CreateNtupleDColumn("18_SiPM");//17
  analysisManager->CreateNtupleDColumn("19_SiPM");//18
  analysisManager->CreateNtupleDColumn("20_SiPM");//19
  analysisManager->CreateNtupleDColumn("21_SiPM");//20
  analysisManager->CreateNtupleDColumn("22_SiPM");//21
  analysisManager->CreateNtupleDColumn("23_SiPM");//22
  analysisManager->CreateNtupleDColumn("24_SiPM");//23
  analysisManager->CreateNtupleDColumn("25_SiPM");//24
  analysisManager->CreateNtupleDColumn("26_SiPM");//25
  analysisManager->CreateNtupleDColumn("27_SiPM");//26
  analysisManager->CreateNtupleDColumn("28_SiPM");//27
  analysisManager->CreateNtupleDColumn("29_SiPM");//28
  analysisManager->CreateNtupleDColumn("30_SiPM");//29
  analysisManager->CreateNtupleDColumn("31_SiPM");//30
  analysisManager->CreateNtupleDColumn("32_SiPM");//31
  analysisManager->CreateNtupleDColumn("33_SiPM");//32
  analysisManager->CreateNtupleDColumn("34_SiPM");//33
  analysisManager->CreateNtupleDColumn("35_SiPM");//34
  analysisManager->CreateNtupleDColumn("36_SiPM");//35
  analysisManager->CreateNtupleDColumn("37_SiPM");//36
  analysisManager->CreateNtupleDColumn("38_SiPM");//37
  analysisManager->CreateNtupleDColumn("39_SiPM");//38
  analysisManager->CreateNtupleDColumn("40_SiPM");//39
  
  analysisManager->CreateNtupleDColumn("WholePhoton");
  analysisManager->CreateNtupleDColumn("WholeSiPM");
  
  

  analysisManager->FinishNtuple();
 



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* )
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "HAWL2.root";
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

 
  const MyDetectorConstruction* detConstruction
   = static_cast<const MyDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  
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

   G4cout
       << G4endl
       << " The run consists of " << nofEvents << " "<< runCondition
       << G4endl
       << "------------------------------------------------------------"
       << G4endl;

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  // G4cout<<"루트파일 만들어짐"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::printEventproc()
{

    fEvent += 1;
  if(fEvent % 10 == 0)
    {
      G4cout<<"처리 Events : "<<fEvent<<"개"<<G4endl;
    }
  
}
