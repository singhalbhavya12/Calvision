# Only run for source packaging
if(CPACK_SOURCE_INSTALLED_DIRECTORIES)
  # Copy any additional files needed in source package that we need to generate
  # or otherwise aren't present directly in the source directory. 
  set(GEANT4_CPACK_SOURCE_EXTRAS "/Users/bs/Desktop/Calvision/Simulation/Geant4/geant4-v11.3.0-build/_source_extras")
  file(INSTALL "${GEANT4_CPACK_SOURCE_EXTRAS}/" DESTINATION "${CMAKE_INSTALL_PREFIX}")

  # Cross-check that we are not packaging up a modified source dir, and
  # that there are no additional untracked files since we configured.
  include("/Users/bs/Desktop/Calvision/Simulation/Geant4/geant4-v11.3.0/cmake/Modules/G4GitUtilities.cmake")
  geant4_git_find_dirty("/Users/bs/Desktop/Calvision/Simulation/Geant4/geant4-v11.3.0" _currently_modded _currently_untracked)

  if(_currently_modded)
    message(WARNING "Modified files found in source directory: run `git status` for more information on these files.")
  endif()

  foreach(_ut ${_currently_untracked})
    list(FIND CPACK_SOURCE_IGNORE_FILES "${_ut}" _is_found)
    if(_is_found LESS 0)
      message(WARNING "New untracked file found in source tree: '${_ut}'. Re-run cmake before trying to package, or remove the file.")
    endif()
  endforeach()
endif()
