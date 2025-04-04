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
//! \file G4GlobalConfig.hh
//! \brief Public global preprocessor defines

#ifndef G4GLOBALCONFIG_HH
#define G4GLOBALCONFIG_HH

//! \def G4MULTITHREADED
//! \brief Defined if Geant4 is built with Multithreading support
#define G4MULTITHREADED

//! \def G4_STORE_TRAJECTORY
//! \brief Defined if Geant4 is built with trajectory storage
#define G4_STORE_TRAJECTORY

//! \def G4VERBOSE
//! \brief Defined if Geant4 is built with additional verbosity in logging
#define G4VERBOSE

//! \def GEANT4_USE_TBB
//! \brief Defined if Geant4 built with TBB support
/* #undef GEANT4_USE_TBB */

#endif // G4GLOBALCONFIG_HH

