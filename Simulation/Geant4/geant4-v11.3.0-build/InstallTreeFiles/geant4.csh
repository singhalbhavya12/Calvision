#!/bin/csh

#-----------------------------------------------------------------------
# C shell environment setup script for Geant4 11.3.0
#
# This script will configure your environment so that any Geant4 supplied
# tools, libraries and data are available in your PATH, library paths etc.
# Data libraries will only be setup if they were installed as part of the
# Geant4 installation.
#
# Source this script to perform the setup.
#
# This script is autogenerated by CMake DO NOT EDIT
#

#-----------------------------------------------------------------------
# Locate directory of self
#
# Self locate script when sourced
# If sourced interactively, we can use $_ as this should be
#
#   source path_to_script_dir/geant4.csh
#
unset g4sls_sourced_dir
unset geant4_envbindir

set ARGS=($_)
if ("$ARGS" != "") then
  if ("$ARGS[2]" =~ */geant4.csh) then
    set g4sls_sourced_dir="`dirname ${ARGS[2]}`"
  endif
endif

if (! $?g4sls_sourced_dir) then
  # Oh great, we were sourced non-interactively. This means that $_
  # won't be set, so we need an external source of information on
  # where the script is located.
  # We obtain this in one of two ways:
  #   1) Current directory:
  #     cd script_dir ; source geant4.csh
  #
  #   2) Supply the directory as an argument to the script:
  #     source script_dir/geant4.csh script_dir
  #
  if ( -e geant4.csh ) then
    set g4sls_sourced_dir="`pwd`"
  else if ( "$1" != "" )  then
    if ( -e ${1}/geant4.csh ) then
      set g4sls_sourced_dir=${1}
    else
      echo "ERROR ${1} does not contain a Geant4 installation"
    endif
  endif
endif

if (! $?g4sls_sourced_dir) then
  echo "ERROR: geant4.csh could NOT self-locate Geant4 installation"
  echo "because it was sourced (i.e. embedded) in another script."
  echo "This is due to limitations of (t)csh but can be worked around by providing"
  echo "the directory where geant4.csh is located"
  echo "to it, either via cd-ing to the directory before sourcing:"
  echo "  cd where_script_is ; source geant4.csh"
  echo "or by supplying the directory as an argument to the script:"
  echo "  source where_script_is/geant4.csh where_script_is"
  echo " "
  exit 1
endif

set geant4_envbindir="`cd ${g4sls_sourced_dir} > /dev/null ; pwd`"


#-----------------------------------------------------------------------
# Setup Geant4 binary and library paths...
#

if ( ! ${?PATH} ) then
  setenv PATH "$geant4_envbindir"
else
  setenv PATH "$geant4_envbindir":${PATH}
endif
      


#-----------------------------------------------------------------------
# Setup Third-party binary and library paths...
# - CLHEP
# - Builtin CLHEP used

# - XercesC
# GDML SUPPORT NOT AVAILABLE

#-----------------------------------------------------------------------
# Resource file paths
# - Datasets
setenv GEANT4_DATA_DIR "`cd $geant4_envbindir/../share/Geant4/data > /dev/null ; pwd`"

# - Variables for individual datasets
# Uncomment the line and edit the path to the dataset if installed in not standard location.

# setenv G4NEUTRONHPDATA $GEANT4_DATA_DIR/G4NDL4.7.1
# setenv G4LEDATA $GEANT4_DATA_DIR/G4EMLOW8.6.1
# setenv G4LEVELGAMMADATA $GEANT4_DATA_DIR/PhotonEvaporation6.1
# setenv G4RADIOACTIVEDATA $GEANT4_DATA_DIR/RadioactiveDecay6.1.2
# setenv G4PARTICLEXSDATA $GEANT4_DATA_DIR/G4PARTICLEXS4.1
# setenv G4PIIDATA $GEANT4_DATA_DIR/G4PII1.3
# setenv G4REALSURFACEDATA $GEANT4_DATA_DIR/RealSurface2.2
# setenv G4SAIDXSDATA $GEANT4_DATA_DIR/G4SAIDDATA2.0
# setenv G4ABLADATA $GEANT4_DATA_DIR/G4ABLA3.3
# setenv G4INCLDATA $GEANT4_DATA_DIR/G4INCL1.2
# setenv G4ENSDFSTATEDATA $GEANT4_DATA_DIR/G4ENSDFSTATE3.0
# setenv G4CHANNELINGDATA $GEANT4_DATA_DIR/G4CHANNELING1.0


# - Fonts for Freetype
# FREETYPE SUPPORT NOT AVAILABLE

#----------------------------------------------------------------------

# unset local variables
unset g4sls_sourced_dir
unset geant4_envbindir

