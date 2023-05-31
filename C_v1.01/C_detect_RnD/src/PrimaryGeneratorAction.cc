#include "PrimaryGeneratorAction.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic

  G4String particle_name = "e+";
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(particle_name);

  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleEnergy(2.0*MeV);

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  
  G4double energy = 2.0*MeV;
  
  G4double radius = 0.05 * m; // radius of circular trajectory
  G4double phi = G4UniformRand() * 2.0 * CLHEP::pi; // random angle
  G4double x_p = radius * G4UniformRand()* std::cos(phi);
  G4double y_p = radius * G4UniformRand()* std::sin(phi);
  G4double z_p = 0.0 *cm;
  
  G4double x_m = G4UniformRand()*2-1; // Random x between -1 and 1
  G4double y_m = G4UniformRand()*2-1; // Random y between -1 and 1
  G4double z_m = G4UniformRand()*2-1; // Random z between -1 and 1


  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticlePosition(G4ThreeVector(x_p, y_p, z_p));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_m, y_m, z_m));

  fParticleGun->GeneratePrimaryVertex(anEvent);  
   
}
