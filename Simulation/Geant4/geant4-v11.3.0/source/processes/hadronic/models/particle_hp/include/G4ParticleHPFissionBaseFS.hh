//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// P. Arce, June-2014 Conversion neutron_hp to particle_hp
//
#ifndef G4ParticleHPFissionBaseFS_h
#define G4ParticleHPFissionBaseFS_h 1

#include "G4Cache.hh"
#include "G4DynamicParticleVector.hh"
#include "G4ParticleHPAngular.hh"
#include "G4ParticleHPEnergyDistribution.hh"
#include "G4ParticleHPFinalState.hh"
#include "G4ParticleHPNames.hh"
#include "G4ParticleHPVector.hh"
#include "G4ReactionProduct.hh"
#include "globals.hh"

class G4ParticleHPFissionBaseFS : public G4ParticleHPFinalState
{
    struct toBeCached
    {
        const G4ReactionProduct* theNeutronRP{nullptr};
        const G4ReactionProduct* theTarget{nullptr};
        toBeCached() = default;
    };

  public:

    G4ParticleHPFissionBaseFS()
    {
      hasXsec = true;
      theXsection = new G4ParticleHPVector;
    }

    ~G4ParticleHPFissionBaseFS() override { delete theXsection; }

    void Init(G4double A, G4double Z, G4int M, const G4String& dirName,
              const G4String& bit, G4ParticleDefinition*) override;

    G4DynamicParticleVector* ApplyYourself(G4int Prompt);

    G4double GetXsec(G4double anEnergy) const override
    {
      return std::max(0., theXsection->GetY(anEnergy));
    }

    G4ParticleHPVector* GetXsec() const override { return theXsection; }

    inline void SetNeutronRP(const G4ReactionProduct& aNeutron)
    {
      fCache.Get().theNeutronRP = &aNeutron;
      theAngularDistribution.SetProjectileRP(aNeutron);
    }

    inline void SetTarget(const G4ReactionProduct& aTarget)
    {
      fCache.Get().theTarget = &aTarget;
      theAngularDistribution.SetTarget(aTarget);
    }

  private:

    G4HadFinalState* ApplyYourself(const G4HadProjectile&) override { return nullptr; }

    G4ParticleHPVector* theXsection;
    G4ParticleHPEnergyDistribution theEnergyDistribution;
    G4ParticleHPAngular theAngularDistribution;

    G4Cache<toBeCached> fCache;
};

#endif
