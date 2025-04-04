# - Geant4 CMake Versioning File for External Projects
# This file is configured by Geant4 for use with CMake's find_package in
# Config mode. It enables external projects to determine the version of 
# Geant4 found. More importantly, it allows that command to automatically 
# determine whether the found install is compatible with any VERSION that 
# was supplied with find_package.
#
# This file is configured by Geant4's CMake system and SHOULD NOT BE EDITED
#-----------------------------------------------------------------------

# - Installed Version as set from CMake
set(PACKAGE_VERSION "11.3.0")
 
#-----------------------------------------------------------------------
# Check whether the requested PACKAGE_FIND_VERSION is compatible with this
# installed version.
if("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
  set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
  if("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
    set(PACKAGE_VERSION_EXACT TRUE)
  endif()
endif()

