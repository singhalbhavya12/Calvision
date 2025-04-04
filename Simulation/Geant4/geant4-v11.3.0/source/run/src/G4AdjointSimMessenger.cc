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
// G4AdjointSimMessenger implementation
//
// --------------------------------------------------------------------
//   Class Name:   G4AdjointSimMessenger
//   Author:       L. Desorgher, 2007-2009
//   Organisation: SpaceIT GmbH
//   Contract:     ESA contract 21435/08/NL/AT
//   Customer:     ESA/ESTEC
// --------------------------------------------------------------------

#include "G4AdjointSimMessenger.hh"

#include "G4AdjointSimManager.hh"
#include "G4RunManager.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"
#include "G4UnitsTable.hh"

#include <sstream>

// --------------------------------------------------------------------
//
G4AdjointSimMessenger::G4AdjointSimMessenger(G4AdjointSimManager* pAdjointRunManager)
  : theAdjointRunManager(pAdjointRunManager)
{
  AdjointSimDir = new G4UIdirectory("/adjoint/");
  AdjointSimDir->SetGuidance("Control of the adjoint or reverse monte carlo simulation");

  // Start and adjoint Run
  //---------------------


  beamOnCmd = new G4UIcommand("/adjoint/start_run", this);
  beamOnCmd->SetGuidance("Start an adjoint Run.");
  beamOnCmd->SetGuidance("Default number of events to be processed is 1.");
  beamOnCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  auto p1 = new G4UIparameter("numberOfEvent", 'i', true);
  p1->SetDefaultValue(1);
  p1->SetParameterRange("numberOfEvent >= 0");
  beamOnCmd->SetParameter(p1);

  // Commands to define parameters relative to the external source
  //------------------------------------------------------------

  auto pos_x_par = new G4UIparameter("X", 'd', true);

  auto pos_y_par = new G4UIparameter("Y", 'd', true);

  auto pos_z_par = new G4UIparameter("Z", 'd', true);

  auto radius_par = new G4UIparameter("R", 'd', true);

  radius_par->SetParameterRange("R >= 0");

  auto unit_par = new G4UIparameter("unit", 's', true);

  DefineSpherExtSourceCmd = new G4UIcommand("/adjoint/DefineSphericalExtSource", this);
  DefineSpherExtSourceCmd->SetGuidance("Define a spherical external source.");
  DefineSpherExtSourceCmd->SetParameter(pos_x_par);
  DefineSpherExtSourceCmd->SetParameter(pos_y_par);
  DefineSpherExtSourceCmd->SetParameter(pos_z_par);
  DefineSpherExtSourceCmd->SetParameter(radius_par);
  DefineSpherExtSourceCmd->SetParameter(unit_par);

  auto phys_vol_name_par = new G4UIparameter("phys_vol_name", 's', true);

  DefineSpherExtSourceCenteredOnAVolumeCmd =
    new G4UIcommand("/adjoint/DefineSphericalExtSourceCenteredOnAVolume", this);
  DefineSpherExtSourceCenteredOnAVolumeCmd->SetGuidance(
    "Define a spherical external source with the center located at the center "
    "of a "
    "physical volume");
  DefineSpherExtSourceCenteredOnAVolumeCmd->SetParameter(phys_vol_name_par);
  DefineSpherExtSourceCenteredOnAVolumeCmd->SetParameter(radius_par);
  DefineSpherExtSourceCenteredOnAVolumeCmd->SetParameter(unit_par);

  DefineExtSourceOnAVolumeExtSurfaceCmd =
    new G4UIcmdWithAString("/adjoint/DefineExtSourceOnExtSurfaceOfAVolume", this);
  DefineExtSourceOnAVolumeExtSurfaceCmd->SetGuidance(
    "Set the external source on the external surface of a physical volume");
  DefineExtSourceOnAVolumeExtSurfaceCmd->SetParameterName("phys_vol_name", false);

  setExtSourceEMaxCmd = new G4UIcmdWithADoubleAndUnit("/adjoint/SetExtSourceEmax", this);
  setExtSourceEMaxCmd->SetGuidance("Set the maximum  energy of the external source");
  setExtSourceEMaxCmd->SetParameterName("Emax", false);
  setExtSourceEMaxCmd->SetUnitCategory("Energy");
  setExtSourceEMaxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Commands to define the adjoint source
  //------------------------------------------------------------

  DefineSpherAdjSourceCmd = new G4UIcommand("/adjoint/DefineSphericalAdjSource", this);
  DefineSpherAdjSourceCmd->SetGuidance("Define a spherical adjoint source.");
  DefineSpherAdjSourceCmd->SetParameter(pos_x_par);
  DefineSpherAdjSourceCmd->SetParameter(pos_y_par);
  DefineSpherAdjSourceCmd->SetParameter(pos_z_par);
  DefineSpherAdjSourceCmd->SetParameter(radius_par);
  DefineSpherAdjSourceCmd->SetParameter(unit_par);

  DefineSpherAdjSourceCenteredOnAVolumeCmd =
    new G4UIcommand("/adjoint/DefineSphericalAdjSourceCenteredOnAVolume", this);
  DefineSpherAdjSourceCenteredOnAVolumeCmd->SetGuidance(
    "Define a spherical adjoint source with the center located at the center "
    "of a "
    "physical volume");
  DefineSpherAdjSourceCenteredOnAVolumeCmd->SetParameter(phys_vol_name_par);
  DefineSpherAdjSourceCenteredOnAVolumeCmd->SetParameter(radius_par);
  DefineSpherAdjSourceCenteredOnAVolumeCmd->SetParameter(unit_par);

  DefineAdjSourceOnAVolumeExtSurfaceCmd =
    new G4UIcmdWithAString("/adjoint/DefineAdjSourceOnExtSurfaceOfAVolume", this);
  DefineAdjSourceOnAVolumeExtSurfaceCmd->SetGuidance(
    "Set the adjoint source on the external surface of physical volume");
  DefineAdjSourceOnAVolumeExtSurfaceCmd->SetParameterName("phys_vol_name", false);

  setAdjSourceEminCmd = new G4UIcmdWithADoubleAndUnit("/adjoint/SetAdjSourceEmin", this);
  setAdjSourceEminCmd->SetGuidance("Set the minimum energy  of the adjoint source");
  setAdjSourceEminCmd->SetParameterName("Emin", false);
  setAdjSourceEminCmd->SetUnitCategory("Energy");
  setAdjSourceEminCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  setAdjSourceEmaxCmd = new G4UIcmdWithADoubleAndUnit("/adjoint/SetAdjSourceEmax", this);
  setAdjSourceEmaxCmd->SetGuidance("Set the maximum energy of the adjoint source");
  setAdjSourceEmaxCmd->SetParameterName("Emax", false);
  setAdjSourceEmaxCmd->SetUnitCategory("Energy");
  setAdjSourceEmaxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  ConsiderParticleAsPrimaryCmd = new G4UIcmdWithAString("/adjoint/ConsiderAsPrimary", this);
  ConsiderParticleAsPrimaryCmd->SetGuidance("Set the selected particle as primary");
  ConsiderParticleAsPrimaryCmd->SetParameterName("particle", false);
  ConsiderParticleAsPrimaryCmd->SetCandidates("e- gamma proton ion");

  NeglectParticleAsPrimaryCmd = new G4UIcmdWithAString("/adjoint/NeglectAsPrimary", this);
  NeglectParticleAsPrimaryCmd->SetGuidance(
    "Remove the selected particle from the list of primaries");
  NeglectParticleAsPrimaryCmd->SetParameterName("particle", false);
  NeglectParticleAsPrimaryCmd->SetCandidates("e- gamma proton ion");

  setNbOfPrimaryFwdGammasPerEventCmd =
    new G4UIcmdWithAnInteger("/adjoint/SetNbOfPrimaryFwdGammasPerEvent", this);
  setNbOfPrimaryFwdGammasPerEventCmd->SetGuidance(
    "Set the nb of primary fwd gamm  generated on the adjoint source");
  setNbOfPrimaryFwdGammasPerEventCmd->SetParameterName("Nb_gammas", false);
  setNbOfPrimaryFwdGammasPerEventCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  setNbOfPrimaryAdjGammasPerEventCmd =
    new G4UIcmdWithAnInteger("/adjoint/SetNbOfPrimaryAdjGammasPerEvent", this);
  setNbOfPrimaryAdjGammasPerEventCmd->SetGuidance(
    "Set the nb of primary fwd gamm  generated on the adjoint source");
  setNbOfPrimaryAdjGammasPerEventCmd->SetParameterName("Nb_gammas", false);
  setNbOfPrimaryAdjGammasPerEventCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  setNbOfPrimaryAdjElectronsPerEventCmd =
    new G4UIcmdWithAnInteger("/adjoint/SetNbOfPrimaryAdjElectronsPerEvent", this);
  setNbOfPrimaryAdjElectronsPerEventCmd->SetGuidance(
    "Set the nb of primary fwd gamm  generated on the adjoint source");
  setNbOfPrimaryAdjElectronsPerEventCmd->SetParameterName("Nb_gammas", false);
  setNbOfPrimaryAdjElectronsPerEventCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

// --------------------------------------------------------------------
//
G4AdjointSimMessenger::~G4AdjointSimMessenger()
{
  delete setNbOfPrimaryAdjElectronsPerEventCmd;
  delete setNbOfPrimaryAdjGammasPerEventCmd;
  delete setNbOfPrimaryFwdGammasPerEventCmd;
  delete NeglectParticleAsPrimaryCmd;
  delete ConsiderParticleAsPrimaryCmd;
  delete setAdjSourceEmaxCmd;
  delete setAdjSourceEminCmd;
  delete DefineAdjSourceOnAVolumeExtSurfaceCmd;
  delete DefineSpherAdjSourceCenteredOnAVolumeCmd;
  delete DefineSpherAdjSourceCmd;
  delete setExtSourceEMaxCmd;
  delete DefineExtSourceOnAVolumeExtSurfaceCmd;
  delete DefineSpherExtSourceCenteredOnAVolumeCmd;
  delete DefineSpherExtSourceCmd;
  delete beamOnCmd;
  delete AdjointSimDir;
}

// --------------------------------------------------------------------
//
void G4AdjointSimMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == nullptr) return;
  if (command == beamOnCmd) {
    G4int nev;
    const auto nv = (const char*)newValue;
    std::istringstream is(nv);
    is >> nev;
    theAdjointRunManager->RunAdjointSimulation(nev);
  }
  else if (command == ConsiderParticleAsPrimaryCmd) {
    theAdjointRunManager->ConsiderParticleAsPrimary(newValue);
  }
  else if (command == NeglectParticleAsPrimaryCmd) {
    theAdjointRunManager->NeglectParticleAsPrimary(newValue);
  }
  if (command == DefineSpherExtSourceCmd) {
    G4double x, y, z, r;
    G4String unit;
    const auto nv = (const char*)newValue;
    std::istringstream is(nv);
    is >> x >> y >> z >> r >> unit;

    x *= G4UnitDefinition::GetValueOf(unit);
    y *= G4UnitDefinition::GetValueOf(unit);
    z *= G4UnitDefinition::GetValueOf(unit);
    r *= G4UnitDefinition::GetValueOf(unit);
    theAdjointRunManager->DefineSphericalExtSource(r, G4ThreeVector(x, y, z));
  }
  else if (command == DefineSpherExtSourceCenteredOnAVolumeCmd) {
    G4double r;
    G4String vol_name, unit;
    const auto nv = (const char*)newValue;
    std::istringstream is(nv);
    is >> vol_name >> r >> unit;
    r *= G4UnitDefinition::GetValueOf(unit);
    theAdjointRunManager->DefineSphericalExtSourceWithCentreAtTheCentreOfAVolume(r, vol_name);
  }
  else if (command == DefineExtSourceOnAVolumeExtSurfaceCmd) {
    theAdjointRunManager->DefineExtSourceOnTheExtSurfaceOfAVolume(newValue);
  }
  else if (command == setExtSourceEMaxCmd) {
    theAdjointRunManager->SetExtSourceEmax(setExtSourceEMaxCmd->GetNewDoubleValue(newValue));
  }
  else if (command == DefineSpherAdjSourceCmd) {
    G4double x, y, z, r;
    G4String unit;
    const auto nv = (const char*)newValue;
    std::istringstream is(nv);
    is >> x >> y >> z >> r >> unit;

    x *= G4UnitDefinition::GetValueOf(unit);
    y *= G4UnitDefinition::GetValueOf(unit);
    z *= G4UnitDefinition::GetValueOf(unit);
    r *= G4UnitDefinition::GetValueOf(unit);
    theAdjointRunManager->DefineSphericalAdjointSource(r, G4ThreeVector(x, y, z));
  }
  else if (command == DefineSpherAdjSourceCenteredOnAVolumeCmd) {
    G4double r;
    G4String vol_name, unit;
    const auto nv = (const char*)newValue;
    std::istringstream is(nv);
    is >> vol_name >> r >> unit;
    r *= G4UnitDefinition::GetValueOf(unit);
    theAdjointRunManager->DefineSphericalAdjointSourceWithCentreAtTheCentreOfAVolume(r, vol_name);
  }
  else if (command == DefineAdjSourceOnAVolumeExtSurfaceCmd) {
    theAdjointRunManager->DefineAdjointSourceOnTheExtSurfaceOfAVolume(newValue);
  }
  else if (command == setAdjSourceEminCmd) {
    theAdjointRunManager->SetAdjointSourceEmin(setAdjSourceEminCmd->GetNewDoubleValue(newValue));
  }
  else if (command == setAdjSourceEmaxCmd) {
    theAdjointRunManager->SetAdjointSourceEmax(setAdjSourceEmaxCmd->GetNewDoubleValue(newValue));
  }
  else if (command == setNbOfPrimaryFwdGammasPerEventCmd) {
    theAdjointRunManager->SetNbOfPrimaryFwdGammasPerEvent(
      setNbOfPrimaryFwdGammasPerEventCmd->GetNewIntValue(newValue));
  }
  else if (command == setNbOfPrimaryAdjGammasPerEventCmd) {
    theAdjointRunManager->SetNbAdjointPrimaryGammasPerEvent(
      setNbOfPrimaryAdjGammasPerEventCmd->GetNewIntValue(newValue));
  }
  else if (command == setNbOfPrimaryAdjElectronsPerEventCmd) {
    theAdjointRunManager->SetNbAdjointPrimaryElectronsPerEvent(
      setNbOfPrimaryAdjElectronsPerEventCmd->GetNewIntValue(newValue));
  }
}
