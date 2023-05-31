#include "Construction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorConstruction::MyDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorConstruction::~MyDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
  //////////////////   materials   //////////////////////////
  //
  //
  /////complete material////
  //
  G4NistManager *nist = G4NistManager::Instance();
  G4Material* Mat_Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Mat_Teflon = nist->FindOrBuildMaterial("G4_TEFLON");
  //
  //////Define Teflon surface/////////////
  //
  std::vector<G4double> ephoton = { 2.67 * eV, 2.92 * eV, 3.26 * eV }; //입사에너지
  std::vector<G4double> reflectivity = { 1.0, 1.0, 1.0 }; //반사비율
  std::vector<G4double> efficiency = { 0.1, 0.1, 0.1 }; //투과율
  G4MaterialPropertiesTable* Teflon_Surface = new G4MaterialPropertiesTable();
  Teflon_Surface->AddProperty("REFLECTIVITY", ephoton, reflectivity);
  Teflon_Surface->AddProperty("EFFICIENCY", ephoton, efficiency);
  G4OpticalSurface* Tef_roughSurf = new G4OpticalSurface("tef_roughSurf");
  Tef_roughSurf->SetType(dielectric_dielectric);
  Tef_roughSurf->SetModel(unified);
  //Tef_roughSurf->SetFinish(groundfrontpainted);//이거 다른 세팅이면 반사가 안됨. ㅠ 
  Tef_roughSurf->SetSigmaAlpha(0.1);//0~1 : grounded ~ polished
  Tef_roughSurf->SetMaterialPropertiesTable(Teflon_Surface);

  
 


  
  /////Compound material (need to compound)///
  //
  // Define the elements that make up LAB
  G4Element* El_C = nist->FindOrBuildElement("C");
  G4Element* El_H = nist->FindOrBuildElement("H");

  ////Define the chemical formula for LAB and densities of its constituents///
  //
  fMat_LAB = new G4Material("LAB", 0.853*g/cm3, 2);
  fMat_LAB->AddElement(El_C, 18);
  fMat_LAB->AddElement(El_H, 30);
  //
  ///////////////Define LAB's scintillation properties/////////// 
  //
  std::vector<G4double> lab_Energy = { 2.67 * eV, 2.92 * eV, 3.26 * eV };
  std::vector<G4double> lab_SCINT = { 0.25, 0.5 , 1.0 };//emission rate
  std::vector<G4double> lab_RIND  = { 1.52, 1.53, 1.55 };
  std::vector<G4double> lab_ABSL  = { 1000. * cm, 1000. * cm, 1000. * cm };
  fmat_Prop_table_LAB = new G4MaterialPropertiesTable();
  fmat_Prop_table_LAB->AddProperty("SCINTILLATIONCOMPONENT1", lab_Energy, lab_SCINT);//emission rate 
  // fmat_Prop_table_LAB->AddProperty("SCINTILLATIONCOMPONENT2", lab_Energy, lab_SCINT);
  fmat_Prop_table_LAB->AddProperty("RINDEX", lab_Energy, lab_RIND);
  fmat_Prop_table_LAB->AddProperty("ABSLENGTH", lab_Energy, lab_ABSL);
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV);
  fmat_Prop_table_LAB->AddConstProperty("RESOLUTIONSCALE", 1.0);
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns);
  //fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 45. * ns);
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  // fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  fMat_LAB->SetMaterialPropertiesTable(fmat_Prop_table_LAB);
  // Set the Birks Constant for the LXe scintillator
  fMat_LAB->GetIonisation()->SetBirksConstant(0.151 * mm / MeV);





  

  
  
  
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  //////////////////  Detector Define   //////////////////////////
  //
  //

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  
  ///////////// World /////////////////
  //
  //
  G4double world_Size = 3. * m;
  
  G4Box* solide_World = new G4Box("Worldsol",                            //its name
			      .5 * world_Size,                        //its size
			      .5 * world_Size,
			      .5 * world_Size);
  
  G4LogicalVolume* logical_World = new G4LogicalVolume(solide_World,  //its solid
						       Mat_Air,       //its material
						       "Worldlog");      //its name
  
  G4VPhysicalVolume* Phys_World = new G4PVPlacement(0,                //no rotation
						    G4ThreeVector(),  //at (0,0,0)
						    logical_World,    //its logical volume
						    "Worldphy",       //its name
  						    0,                //its mother  volume
						    false,            //no boolean operation
						    0,                //copy number
						    checkOverlaps);   //overlaps checking

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  ////// Teflon cylinder /////
  G4double innerRadius = 0.05 * m;
  G4double outerRadius = 0.055 * m;
  G4double height = 0.2 * m;
  
  G4Tubs* teflonSol = new G4Tubs("Teflonsol",
			      innerRadius,
			      outerRadius,
			      .5 * height,
			      0.,
			      2. * M_PI);
  
  G4LogicalVolume* teflonLog = new G4LogicalVolume(teflonSol,
						   Mat_Teflon,
						   "Teflonlog");
  
  G4ThreeVector teflonPos = G4ThreeVector(0., 0., 0.); // position of Teflon cylinder
  G4VPhysicalVolume* teflonPhy = new G4PVPlacement(nullptr,
						   teflonPos,
						   teflonLog,
						   "Teflonphy",
						   logical_World,
						   false,
						   0,
						   checkOverlaps);

  
  

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  ////// LAB LS /////

   
  G4double LS_innerRadius = 0.;
  G4double LS_outerRadius = 0.05 * m;
  G4double LS_height = 0.2 * m;
  
  G4Tubs* LS_Sol = new G4Tubs("LAB_Sol",
			      LS_innerRadius,
			      LS_outerRadius,
			      .5 *LS_height,
			      0.,
			      2. * M_PI);
  G4LogicalVolume* LS_Log = new G4LogicalVolume(LS_Sol,
						fMat_LAB,
						"LAB_Log");
  
  G4ThreeVector LS_Pos = G4ThreeVector(0., 0., 0.); // position of Teflon cylinder
  G4VPhysicalVolume* LS_Phy = new G4PVPlacement(nullptr,
						LS_Pos,
						LS_Log,
						"LAB_phy",
						logical_World,
						false,
						0,
						checkOverlaps);
  


  /////////Surface define//////////////
  //G4LogicalBorderSurface* LAB_Tef_borderSurface = new G4LogicalBorderSurface("tef_roughSurf",
  //							          	     LS_Phy, teflonPhy,
  //									     Tef_roughSurf);
  G4LogicalSkinSurface* Tef_skinSurface = new G4LogicalSkinSurface("TargetSurface",
							       teflonLog,Tef_roughSurf);
  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////// SD ////////////////////////////
  //

  G4double SD_width = 0.006 * m;
  G4double SD_height = 0.001 * mm;
  G4int N=12;
  G4double sipm_pos=0;
  
  G4Box* SiPM_sol = new G4Box("SiPM_sol",SD_width,SD_height,SD_width);
  
  
  fSipm_SD_LOG = new G4LogicalVolume(SiPM_sol,fMat_LAB,"SiPM_LOG");

  
  for(G4int i=0; i<N; i++)
  {
    sipm_pos = height/2.*((2.*i+1.)/N-1.);
    G4VPhysicalVolume* SiPM_Phy = new G4PVPlacement(0,
						    G4ThreeVector(0., 0., sipm_pos),
						    fSipm_SD_LOG,
						    "SiPM_PHY",
						    LS_Log,
						    false,
						    i,
						    checkOverlaps);
  }
  
  logical_World->SetVisAttributes(G4VisAttributes::GetInvisible());//world 안보이게 함 
  
  return Phys_World;
  
}
/*
void MyDetectorConstruction::ConstructSD()
{
  MySensitiveDetector* SD = new MySensitiveDetector("SiPM_SD");
  fSipm_SD_LOG->SetSensitiveDetector(SD);
}
*/
