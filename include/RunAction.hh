
#ifndef RunAction_h
#define RunAction_h 1


#include "PrimaryGeneratorAction.hh"
#include "Construction.hh"


#include "G4UserRunAction.hh"

#include "G4AnalysisManager.hh" //for make root file

#include "G4RunManager.hh"
#include "G4Run.hh"

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


private:
  G4int fEvent =0;
  G4int nofEvents=0;  
};


#endif

