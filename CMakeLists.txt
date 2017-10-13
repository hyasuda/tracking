#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MUONGM2EDMSIM)

# use of CERN ROOT -----
option(USE_ROOT "include ROOT headers and libraries" ON)
if(USE_ROOT)
  # This allows CMake to find module
  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

  # find ROOT
  find_package(ROOT)

  # Add ROOT header paths
  include_directories(${ROOT_INCLUDE_DIRS})
endif()
  

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(muongm2edmsim muongm2edmsim.cc ${sources} ${headers})

if(USE_ROOT)
  target_link_libraries(muongm2edmsim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
else()
  target_link_libraries(muongm2edmsim ${Geant4_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build muongm2edmsim_v0.1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(MUONGM2EDMSIM_SCRIPTS
  macro/tmp_track_0.mac
  macro/originvis.mac
  FLDATA/20160422_Abe2017May-5.txt
  FLDATA/20160422_Abe2017May-7.txt
  FLDATA/20160422_Abe2017May-77.txt
  FLDATA/20160422_cm12weak.txt
  beamSample/sample_1200_good.txt
  )

foreach(_script ${MUONGM2EDMSIM_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(MUONGM2EDMSIM DEPENDS muongm2edmsim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS muongm2edmsim DESTINATION bin)
