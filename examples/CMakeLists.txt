cmake_minimum_required ( VERSION 3.14 )

# C++ standards
#----------------------------------------------------------
set ( CMAKE_GENERATOR_PLATFORM     x64)
set ( CMAKE_CXX_STANDARD           17 )
set ( CMAKE_CXX_STANDARD_REQUIRED  ON )
set ( CMAKE_CXX_EXTENSIONS         OFF )
set ( MY_CXX_COMPILE_FEATURES      cxx_generic_lambdas cxx_range_for cxx_strong_enums )

# Project
#----------------------------------------------------------
project ( dgm-examples )

# Dependencies
#----------------------------------------------------------
include ( FetchContent )

## Add dependencies
##---------------------------------------------------------
set ( SFML_VERSION "2.5.1" )
set ( SFML_URL "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )

# Fetch and link SFML
FetchContent_Declare ( sfml
	URL "${SFML_URL}"
)
FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
	FetchContent_Populate(sfml)
endif()

set (SFML_FOLDER "${sfml_SOURCE_DIR}")
include_directories("${SFML_FOLDER}/include")
link_directories("${SFML_FOLDER}/lib")

### Copy DLLs to build folders
###--------------------------------------------------------
file (GLOB DEBUG_SFML_DLLS   ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/bin/*-d-2.dll)
file (GLOB RELEASE_SFML_DLLS ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/bin/*[oskmw]-2.dll)
set ( OPENAL_DLL ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/bin/openal32.dll )

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug" "${CMAKE_CURRENT_BINARY_DIR}/Release" )

configure_file (
	${OPENAL_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug/ COPYONLY
)

configure_file (
	${OPENAL_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Release/ COPYONLY
)

foreach ( DLL ${DEBUG_SFML_DLLS} )
	configure_file ( ${DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug/ COPYONLY )
endforeach ( DLL )

foreach ( DLL ${RELEASE_SFML_DLLS} )
	configure_file ( ${DLL} ${CMAKE_CURRENT_BINARY_DIR}/Release/ COPYONLY )
endforeach ( DLL )

## Link DGM
##---------------------------------------------------------
# NOTE: SFML is linked dynamically from DGM
set ( DGM_FOLDER "../.." )
include_directories ( "${DGM_FOLDER}/include" )
link_directories("${DGM_FOLDER}/lib")
#find_library ( LIB_DGM_D dgm-d NAMES dgm-d.lib PATHS "${DGM_FOLDER}/lib" )
#find_library ( LIB_DGM_R dgm   NAMES dgm.lib   PATHS "${DGM_FOLDER}/lib" )
set ( LIB_DGM_D "${DGM_FOLDER}/lib/dgm-d.lib" )
set ( LIB_DGM_R "${DGM_FOLDER}/lib/dgm.lib" )
message ( "debug" )
message ( "${LIB_DGM_D}" )

set ( LIB_DGM optimized ${LIB_DGM_R} debug ${LIB_DGM_D} )

# Examples
#----------------------------------------------------------

## Example 01
##---------------------------------------------------------
add_executable(01_polar_cartesian WIN32 01_polar_cartesian.cpp)
target_link_libraries(01_polar_cartesian ${LIB_DGM})

## Example 02
##---------------------------------------------------------
add_executable(02_controller WIN32 02_controller.cpp)
target_link_libraries(02_controller ${LIB_DGM})

## Example 03
##---------------------------------------------------------
add_executable(03_app_state WIN32 03_app_state.cpp)
target_link_libraries(03_app_state ${LIB_DGM})

## Example 04
##---------------------------------------------------------
add_executable(04_particle_system WIN32 04_particle_system.cpp)
target_link_libraries(04_particle_system ${LIB_DGM})

## Example 05
##---------------------------------------------------------
add_executable(05_game_fifteen WIN32 05_game_fifteen.cpp)
target_link_libraries(05_game_fifteen ${LIB_DGM})

## Example 06
##---------------------------------------------------------
add_executable(06_animation WIN32 06_animation.cpp)
target_link_libraries(06_animation ${LIB_DGM})

## Example 07
##---------------------------------------------------------
add_executable(07_collisions WIN32 07_collisions.cpp)
target_link_libraries(07_collisions ${LIB_DGM})

## Example 08
##---------------------------------------------------------
add_executable(08_buffer_mesh WIN32 08_buffer_mesh.cpp)
target_link_libraries(08_buffer_mesh ${LIB_DGM})

## Example 09
##---------------------------------------------------------
add_executable(09_resource_manager WIN32 09_resource_manager.cpp)
target_link_libraries(09_resource_manager ${LIB_DGM})

## Example 10
##---------------------------------------------------------
add_executable(10_tileset WIN32 10_tileset.cpp)
target_link_libraries(10_tileset ${LIB_DGM})

## Example 11
##---------------------------------------------------------
add_executable(11_advanced_particle_system WIN32 11_advanced_particle_system.cpp)
target_link_libraries(11_advanced_particle_system ${LIB_DGM})