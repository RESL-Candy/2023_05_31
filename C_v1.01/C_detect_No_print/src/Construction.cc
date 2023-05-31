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
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD", 1200. / MeV);
  fmat_Prop_table_LAB->AddConstProperty("RESOLUTIONSCALE", 1.0);
  fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 20. * ns);
  //fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 45. * ns);
  //fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  // fmat_Prop_table_LAB->AddConstProperty("SCINTILLATIONYIELD2", 0.0);
  fMat_LAB->SetMaterialPropertiesTable(fmat_Prop_table_LAB);
  // Set the Birks Constant for the LXe scintillator
  fMat_LAB->GetIonisation()->SetBirksConstant(0.151 * mm / MeV);

  ///////////////Define Teflon's scintillation properties///////////
  //
  //
  /*
  fmat_Prop_table_Teflon = new G4MaterialPropertiesTable();

  std::vector<G4double> teflon_Energy = { 2.67 * eV, 2.92 * eV, 3.26 * eV };
  std::vector<G4double> teflon_RIND  = { 2.00, 2.00, 2.00 };
  std::vector<G4double> teflon_ABSL  = { 1000. * cm, 1000. * cm, 1000. * cm };
  fmat_Prop_table_Teflon->AddProperty("RINDEX", teflon_Energy, teflon_RIND);
  fmat_Prop_table_Teflon->AddProperty("ABSLENGTH", teflon_Energy, teflon_ABSL);  
  Mat_Teflon->SetMaterialPropertiesTable(fmat_Prop_table_Teflon);
  */
  
  
  
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
  G4double world_Size = 10. * m;
  
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
  G4double innerRadius = 0.;
  G4double outerRadius = 0.055 * m;
  G4double height = 0.45 * m;
  
  G4Tubs* teflonSol = new G4Tubs("Teflonsol",
				 innerRadius,
				 outerRadius,
				 .5 * height,
				 0.,         //tub 에서 시작 각도 
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
  
  G4double cone_rmina = 25.*mm;//윗면 안쪽원
  /*
  G4double cone_rmaxa = 30.*mm;//윗면 바깥원
  G4double cone_rminb = 50.*mm;//아랫면 안쪽원
  G4double cone_rmaxb = 55.*mm;//아랫면 바깥원
  G4double cone_h = 60.*mm;//전체길이의 절
  G4double cone_phimin = 0.*deg;
  G4double cone_phimax = 360.*deg;
  */
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  //////Teflon truncated cone/////
  /*
  G4double cone_rmina = 25.*mm;//윗면 안쪽원
  G4double cone_rmaxa = 30.*mm;//윗면 바깥원
  G4double cone_rminb = 50.*mm;//아랫면 안쪽원
  G4double cone_rmaxb = 55.*mm;//아랫면 바깥원
  G4double cone_h = 60.*mm;//전체길이의 절
  G4double cone_phimin = 0.*deg;
  G4double cone_phimax = 360.*deg;
  
  G4Cons*solidcone = new G4Cons("cone",
				cone_rmina,
				cone_rmaxa,
				cone_rminb,
				cone_rmaxb,
				cone_h,
				cone_phimin,
				cone_phimax);
  
  G4LogicalVolume* logiccone = new G4LogicalVolume(solidcone,
						   Mat_Teflon,
						   "cone_log");
  G4VPhysicalVolume* phycone = new G4PVPlacement(nullptr,
						 G4ThreeVector(0.,0.,-165.*mm),
						 logiccone,
						 "cone_phy",
						 logical_World,
						 false,
						 0,
						 checkOverlaps);
  /////////////////////////////////////////////////////////////////////////////////////////

  
  G4double cone_rmina1 = 50.*mm;
  G4double cone_rmaxa1 = 55.*mm;
  G4double cone_rminb1 = 25.*mm;
  G4double cone_rmaxb1 = 30.*mm;
  G4double cone_h1 = 60.*mm;
  G4double cone_phimin1 = 0.*deg;//시작 각
  G4double cone_phimax1 = 360.*deg;//끝 각도 
  
  G4Cons*solidcone1 = new G4Cons("cone",
				cone_rmina1,
				cone_rmaxa1,
				cone_rminb1,
				cone_rmaxb1,
				cone_h1,
				cone_phimin1,
				cone_phimax1);
  
  G4LogicalVolume* logiccone1 = new G4LogicalVolume(solidcone1,
						   Mat_Teflon,
						   "cone_log1");
  G4VPhysicalVolume* phycone1 = new G4PVPlacement(nullptr,
						 G4ThreeVector(0.,0.,165.*mm),
						 logiccone1,
						 "cone_phy1",
						 logical_World,
						 false,
						 0,
						 checkOverlaps);
  
  
  */
  
  

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  ////// LAB LS /////

  
  G4double LS_innerRadius = 0.;
  G4double LS_outerRadius = 0.05 * m;
  G4double LS_height = 0.105 * m;

  G4Tubs* LS_Sol = new G4Tubs("LAB_Sol",
			      LS_innerRadius,
			      LS_outerRadius,
			      .5 *LS_height,
			      0.,
			      2. * M_PI);
  /*
  G4LogicalVolume* LS_Log = new G4LogicalVolume(LS_Sol,
						fMat_LAB,
						"LAB_Log");
  
  G4ThreeVector LS_Pos = G4ThreeVector(0., 0., 0.); // position of LS cylinder
  G4VPhysicalVolume* LS_Phy = new G4PVPlacement(nullptr,
						LS_Pos,
						LS_Log,
						"LAB_phy",
						teflonLog,  //mod 0519
						false,
						0,
						checkOverlaps);
  */
 ///////////////////////////////////ConeLAB_LS///////////////////////////////////////
  G4double cone_rminaLS = 0.*mm;
  G4double cone_rmaxaLS = 50.*mm;
  G4double cone_rminbLS = 0.*mm;
  G4double cone_rmaxbLS = 25.*mm;
  G4double cone_hLS = 60.*mm;
  G4double cone_phiminLS = 0.*deg;
  G4double cone_phimaxLS = 360.*deg;
  
  G4Cons*ConLS_SolLS = new G4Cons("LAB_Cone",
				cone_rminaLS,
				cone_rmaxaLS,
				cone_rminbLS,
				cone_rmaxbLS,
				cone_hLS,
				cone_phiminLS,
				cone_phimaxLS);
  /*
  G4LogicalVolume* ConeLS_Log = new G4LogicalVolume(ConLS_SolLS,
						    fMat_LAB,
						    "ConeLAB_Log");
  G4VPhysicalVolume* ConeLS_Phy = new G4PVPlacement(nullptr,
						    G4ThreeVector(0.,0.,-165.*mm),
						    ConeLS_Log,
						    "ConeLAB_Log",
						    teflonLog,
						    false,
						    0,
						    checkOverlaps);
  */
  ///////////////////////////////////////////////////////////////////////////////////////////
  G4double cone_rmaxaLS1 = 25.*mm;
  /*
  G4double cone_rminaLS1 = 0.*mm;

  G4double cone_rminbLS1 = 0.*mm;
  G4double cone_rmaxbLS1 = 50.*mm;
  G4double cone_hLS1 = 60.*mm;
  G4double cone_phiminLS1 = 0.*deg;
  G4double cone_phimaxLS1 = 360.*deg;
  
  G4Cons*ConLS_SolLS1 = new G4Cons("LAB_Cone1",
				 cone_rminaLS1,
				 cone_rmaxaLS1,
				 cone_rminbLS1,
				 cone_rmaxbLS1,
				 cone_hLS1,
				 cone_phiminLS1,
				 cone_phimaxLS1);
  */
  

  G4UnionSolid* unionSolidLS = new G4UnionSolid("unionSolidLS",LS_Sol,ConLS_SolLS,0,G4ThreeVector(0,0,112.5*mm));
  G4LogicalVolume* logicalUnion = new G4LogicalVolume(unionSolidLS,
						      fMat_LAB,
						      "unionLAB");
  /*
  G4LogicalVolume* ConeLS_Log1 = new G4LogicalVolume(ConLS_SolLS1,
						     fMat_LAB,
						     "ConeLAB_Log1");
  */
  G4RotationMatrix* rotationX = new G4RotationMatrix();
  rotationX -> rotateX(180.0*deg);

  G4VPhysicalVolume* ConeLS_Phy = new G4PVPlacement(0,
						    G4ThreeVector(0.,0.,52.5*mm),
						    logicalUnion,
						    "ConeLAB_Log1",
						    teflonLog,
						    false,
						    0,
						    checkOverlaps);
  
  G4VPhysicalVolume* ConeLS_Phy1 = new G4PVPlacement(rotationX,
						     G4ThreeVector(0.,0.,-52.5*mm),
						     logicalUnion,
						     "ConeLAB_Log1",
						     teflonLog,
						     false,
						     0,
						     checkOverlaps);
  
  /*
   /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //
  //////Teflon truncated cone/////

  G4double cone_rmina = 25.*mm;//윗면 안쪽원
  G4double cone_rmaxa = 30.*mm;//윗면 바깥원
  G4double cone_rminb = 50.*mm;//아랫면 안쪽원
  G4double cone_rmaxb = 55.*mm;//아랫면 바깥원
  G4double cone_h = 60.*mm;//전체길이의 절
  G4double cone_phimin = 0.*deg;
  G4double cone_phimax = 360.*deg;
  
  G4Cons*solidcone = new G4Cons("cone",
				cone_rmina,
				cone_rmaxa,
				cone_rminb,
				cone_rmaxb,
				cone_h,
				cone_phimin,
				cone_phimax);
  
  G4LogicalVolume* logiccone = new G4LogicalVolume(solidcone,
						   Mat_Teflon,
						   "cone_log");
  G4VPhysicalVolume* phycone = new G4PVPlacement(nullptr,
						 G4ThreeVector(0.,0.,-165.*mm),
						 logiccone,
						 "cone_phy",
						 LS_Log,
						 false,
						 0,
						 checkOverlaps);
  /////////////////////////////////////////////////////////////////////////////////////////

  
  G4double cone_rmina1 = 50.*mm;
  G4double cone_rmaxa1 = 55.*mm;
  G4double cone_rminb1 = 25.*mm;
  G4double cone_rmaxb1 = 30.*mm;
  G4double cone_h1 = 60.*mm;
  G4double cone_phimin1 = 0.*deg;//시작 각
  G4double cone_phimax1 = 360.*deg;//끝 각도 
  
  G4Cons*solidcone1 = new G4Cons("cone",
				cone_rmina1,
				cone_rmaxa1,
				cone_rminb1,
				cone_rmaxb1,
				cone_h1,
				cone_phimin1,
				cone_phimax1);
  
  G4LogicalVolume* logiccone1 = new G4LogicalVolume(solidcone1,
						   Mat_Teflon,
						   "cone_log1");
  G4VPhysicalVolume* phycone1 = new G4PVPlacement(nullptr,
						 G4ThreeVector(0.,0.,165.*mm),
						 logiccone1,
						 "cone_phy1",
						 LS_Log,
						 false,
						 0,
						 checkOverlaps);
  
  

  */
  

  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////// PMT ////////////////////////////
  //
  
  G4double PMT_height = 0.001 * m;

  G4double PMT_pos_z=0.;
  
  G4Tubs* PMT_Sol = new G4Tubs("PMT_Sol",
			       0,
			       cone_rmina,
			       .5 *PMT_height,
			       0.,
			       2. * M_PI);

  fPMT_SD_LOG = new G4LogicalVolume(PMT_Sol,
				    fMat_LAB,
				    "PMT_Log");

  // position of PMT
  

  for(G4int i=0; i<2; i++)
    {
      PMT_pos_z=(LS_height*2+ cone_hLS*4-PMT_height)*0.5*(2*i-1);     
      //G4VPhysicalVolume* PMT_Phy =
      new G4PVPlacement(nullptr,
			G4ThreeVector(0., 0., PMT_pos_z),
			fPMT_SD_LOG,
			"PMT_phy",
			teflonLog,  
			false,
			i,
			checkOverlaps);
      
    }
  
  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////// SD ////////////////////////////
  //

  G4double SD_width = 0.006 * m;
  G4double SD_height = 0.001 * mm;
  G4int N=12;
  G4double sipm_pos=0;
  
  G4Box* SiPM_sol = new G4Box("SiPM_sol",.5*SD_width,.5*SD_height,.5*SD_width);
  
  
  fSipm_SD_LOG = new G4LogicalVolume(SiPM_sol,fMat_LAB,"SiPM_LOG");

  
  for(G4int i=0; i<N; i++)
    {
      sipm_pos = (LS_height-PMT_height)/2.*((2.*i+1.)/N-1.);
      //G4VPhysicalVolume* SiPM_Phy =
      new G4PVPlacement(0,
			G4ThreeVector(0., 0., sipm_pos),
			fSipm_SD_LOG,
			"SiPM_PHY",
			logicalUnion,
			false,
			i,
			checkOverlaps);
    }
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////광섬유///////////////////////////////////////////////

  G4double lightline_length = 100.*mm;
  G4double lightline_R = 0.006*m;
  G4int H=12;
  G4double LL_pos=0;


  G4Tubs* line_sol = new G4Tubs("line_sol",0.*mm,.25*lightline_R,.25*lightline_length,0,360.*deg);
  
  fLL_LOG = new G4LogicalVolume(line_sol,fMat_LAB,"LL_LOG");
  G4RotationMatrix* rotation = new G4RotationMatrix();
  rotation -> rotateX(90.0*deg);

  for(G4int i=0; i<H; i++)
    {
      LL_pos = (LS_height-PMT_height)/2.*((2.*i+1.)/H-1.);
      new G4PVPlacement(rotation,
			G4ThreeVector(0.,(cone_rmaxaLS1-.5*SD_height),LL_pos),
			fLL_LOG,
			"LL_PHY",
			logicalUnion,
			false,
			i,
			checkOverlaps);
    }
  for(G4int i=0; i<H; i++)
    {
      LL_pos = (LS_height-PMT_height)/2.*((2.*i+1.)/H-1.);
      new G4PVPlacement(rotation,
			G4ThreeVector(0.,-(cone_rmaxaLS1-.5*SD_height),LL_pos),
			fLL_LOG,
			"LL_PHY",
			logicalUnion,
			false,
			i,
			checkOverlaps);
    }
  

  

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////Surface define////////////////////////////////////
  //
  //
  //
  //
  //
  //////Define Teflon surface/////////////
  //

  /*   //Geant4 Application manual의 Listing 73에 dielectric_dielectric의 경우가 적혀있음
  //또한 teflon의 material property 또한 지정해줘야함.
  //그래서 LUT모델을 씀.
  std::vector<G4double> ephoton = { 2.67 * eV, 2.92 * eV, 3.26 * eV }; //입사에너지
  std::vector<G4double> reflectivity = { 1.0, 1.0, 1.0 }; //반사비율
  std::vector<G4double> efficiency = { 0.0, 0.0, 0.0 }; //투과율
  G4MaterialPropertiesTable* Teflon_Surface = new G4MaterialPropertiesTable();
  Teflon_Surface->AddProperty("REFLECTIVITY", ephoton, reflectivity);
  Teflon_Surface->AddProperty("EFFICIENCY", ephoton, efficiency);
  */




  
  G4OpticalSurface* Tef_roughSurf = new G4OpticalSurface("tef_roughSurf");
  Tef_roughSurf->SetType(dielectric_LUTDAVIS); //이거 하면 LUT를 사용하여 데이터를 갖고와서 씀. 
  Tef_roughSurf->SetModel(DAVIS);// LUT DAVIS 모델은 DAVIS 고정
  Tef_roughSurf->SetFinish(RoughTeflon_LUT);//이거 다른 세팅이면 반사가 안됨. --> 해결!  

  new G4LogicalBorderSurface("tef_roughSurf",
			     ConeLS_Phy1, teflonPhy,
			     Tef_roughSurf);
  new G4LogicalBorderSurface("tef_roughSurf",
			     ConeLS_Phy, teflonPhy,
			     Tef_roughSurf);


  //Tef_roughSurf->SetSigmaAlpha(0.1);//0~1 : grounded ~ polished
  //Tef_roughSurf->SetMaterialPropertiesTable(Teflon_Surface);



  
  //G4LogicalSkinSurface* Tef_skinSurface = new G4LogicalSkinSurface("TargetSurface",
  //								   teflonLog,Tef_roughSurf);

  //위의 skin과 border의 차이는 단순 표면 그리고 경계의 차이이다.










  
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
