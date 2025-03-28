# This file is configured at cmake time, and loaded at cpack time.
# To pass variables to cpack from cmake, they must be configured
# in this file.  

# - NSIS Specifics
if(CPACK_GENERATOR STREQUAL "NSIS")
  # There is a bug in NSI that does not handle full unix paths properly. 
  # Make sure there is at least one set of two (2) backlasshes.
  set(CPACK_PACKAGE_ICON "/Users/bs/Desktop/Calvision/Simulation/Geant4/geant4-v11.3.0\\cmake\\Templates\\g4_small.bmp")
  set(CPACK_NSIS_MUI_ICON "/Users/bs/Desktop/Calvision/Simulation/Geant4/geant4-v11.3.0\\cmake\\Templates\\G4.ico")
  set(CPACK_NSIS_MUI_UNIICON "/Users/bs/Desktop/Calvision/Simulation/Geant4/geant4-v11.3.0\\cmake\\Templates\\G4.ico")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
  set(CPACK_NSIS_HELP_LINK "http:\\\\www.geant4.org")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\www.geant4.org")
  set(CPACK_NSIS_CONTACT "gabriele.cosmo@cern.ch")
  set(CPACK_NSIS_MODIFY_PATH ON)
endif()

# - productbuild Specifics
if(CPACK_GENERATOR STREQUAL "productbuild")
  set(CPACK_PACKAGING_INSTALL_PREFIX "/Library/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
endif()
