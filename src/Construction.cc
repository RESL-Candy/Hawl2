#include "Construction.hh"
#include "G4NistManager.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}


G4VPhysicalVolume* MyDetectorConstruction ::Construct()
{
  G4NistManager *nist = G4NistManager::Instance();
  
  G4Element* El_C = nist->FindOrBuildElement("C");
  G4Element* El_H = nist->FindOrBuildElement("H");
  
  ////Define the chemical formula for LAB and densities of its constituents///
  //
  fMat_LAB = new G4Material("LAB", 0.853*g/cm3, 2);
  fMat_LAB->AddElement(El_C, 18);
  fMat_LAB->AddElement(El_H, 30);
  
  std::vector<G4double> lab_Energy = { 2.67 * eV, 2.92 * eV, 3.26 * eV };
  std::vector<G4double> lab_SCINT = { 0.25, 0.5 , 1.0 };//emission rate
  std::vector<G4double> lab_RIND  = { 1.52, 1.53, 1.55 };
  std::vector<G4double> lab_ABSL  = { 1000. * cm, 1000. * cm, 1000. * cm };
  
  fmat_Prop_table_LAB = new G4MaterialPropertiesTable();
  fmat_Prop_table_LAB->AddProperty("SCINTILLATIONCOMPONENT1", lab_Energy, lab_SCINT);//emission rate 
  fmat_Prop_table_LAB->AddProperty("RINDEX", lab_Energy, lab_RIND);
  fmat_Prop_table_LAB->AddProperty("ABSLENGTH", lab_Energy, lab_ABSL);
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV);
  fmat_Prop_table_LAB->AddConstProperty("RESOLUTIONSCALE", 1.0);
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns);
  
  fMat_LAB->SetMaterialPropertiesTable(fmat_Prop_table_LAB);
  
  fMat_LAB->GetIonisation()->SetBirksConstant(0.151 * mm / MeV);


  G4double world_Size = 3. * m;
  
  G4Box* worldSolid = new G4Box("Worldsol",                            //its name
			      .5 * world_Size,                        //its size
			      .5 * world_Size,
			      .5 * world_Size);

  G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"), "WorldLogical");

  G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, 0);


  // Define the dimensions of the water cuboid
  G4double LABX = 1. * m ; // Half-length in X direction (cm)
  G4double LABY = 0.5 * m; // Half-length in Y direction (cm)
  G4double LABZ = 0.2 * m; // Half-length in Z direction (cm)
    
  // Create the water material (you can use G4NistManager to get predefined materials)
  //G4Material* water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  
  // Create the water cuboid volume
  G4Box* LABSol = new G4Box("LABsol", .5 * LABX, .5 * LABY, .5 * LABZ);
  
  G4LogicalVolume* LABLog = new G4LogicalVolume(LABSol, fMat_LAB, "LABlog");
  
  G4ThreeVector LABPos = G4ThreeVector(0., 0., 0.);

  G4VPhysicalVolume* LABPhy = new G4PVPlacement(nullptr,
						LABPos,
						LABLog,
						"LABphy",
						worldLogical,
						false,
						0);


  

  G4double SD_width = 0.006 * m; //6mm
  G4double SD_height = 0.001 * m; //1mm
  G4int SiPM_N=10;
  G4double sipm_pos=0;
  
  G4Box* SiPM_sol = new G4Box("SiPM_sol",.5*SD_width,.5*SD_width,.5*SD_height);
  
  
  fSipm_SD_LOG = new G4LogicalVolume(SiPM_sol,fMat_LAB,"SiPM_LOG");
  
  for(G4int i=0; i<SiPM_N; i++)
    {
      sipm_pos = ( (i+1) * LABX/(SiPM_N+1) ) - LABX/2;
      new G4PVPlacement(0,
			G4ThreeVector(sipm_pos, 0.15 * m , ( 0.1 - 0.001) * m  ),
			fSipm_SD_LOG,
			"SiPM_Phy",
			LABLog,
			false,
			i);
      
      new G4PVPlacement(0,
			G4ThreeVector(sipm_pos,  0.05 * m , ( 0.1 - 0.001) * m ),
			fSipm_SD_LOG,
			"SiPM_Phy",
			LABLog,
			false,
			i+10);

      new G4PVPlacement(0,
			G4ThreeVector(sipm_pos,  -0.05 * m , ( 0.1 - 0.001) * m ),
			fSipm_SD_LOG,
			"SiPM_Phy",
			LABLog,
			false,
			i+20);
      new G4PVPlacement(0,
			G4ThreeVector(sipm_pos,  -0.15 * m , ( 0.1 - 0.001) * m ),
			fSipm_SD_LOG,
			"SiPM_Phy",
			LABLog,
			false,
			i+30);
	    
    }
 
  // Create the optical surface
  G4OpticalSurface* opticalSurface = new G4OpticalSurface("LABSurface");
  
  // Set the type of the surface (dielectric-dielectric)
  opticalSurface->SetType(dielectric_dielectric);
  
  // Set the finish of the surface (polished for perfect reflection)
  opticalSurface->SetFinish(polished);
  
  // Set the model of the surface (glisur for polished surfaces)
  opticalSurface->SetModel(glisur);
  
  // Set the properties for total internal reflection
  G4double refractiveIndexWater = 1.33; // Refractive index of water
  G4double refractiveIndexAir = 1.0;   // Refractive index of air
  G4double energyMin = 2.0*eV;         // Minimum photon energy for reflection
  
  G4MaterialPropertiesTable* opticalSurfaceProperties = new G4MaterialPropertiesTable();

  G4MaterialPropertyVector* reflectivity = new G4MaterialPropertyVector();

  
  
  opticalSurfaceProperties->AddProperty("REFLECTIVITY", 
				        reflectivity,true);
  opticalSurfaceProperties->AddProperty("SPECULARLOBECONSTANT", 
					new G4MaterialPropertyVector());
  opticalSurfaceProperties->AddProperty("SPECULARSPIKECONSTANT", 
					new G4MaterialPropertyVector());
  opticalSurfaceProperties->AddProperty("BACKSCATTERCONSTANT", 
					new G4MaterialPropertyVector());
  
  // Set the reflectivity to 100% for all photon energies
  //opticalSurfaceProperties->AddConstProperty("REFLECTIVITY", 1.0);
  
  opticalSurface->SetMaterialPropertiesTable(opticalSurfaceProperties);

  G4LogicalBorderSurface* waterSurface = new G4LogicalBorderSurface("LABSurface",
                                                                 LABPhy, worldPhysical,
                                                                 opticalSurface);

  
  
  
  return worldPhysical;
}

