cmake_minimum_required(VERSION 3.10)

# Compiler setup
#----------------------------------------------------------
set (CMAKE_GENERATOR_PLATFORM "x64")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)

if (NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Project setup
#----------------------------------------------------------
project (dgm)
include ( version.cmake )
string ( TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER )

# Dependencies
#----------------------------------------------------------
include ( FetchContent )
include ( dependencies.cmake )

## Add dependencies
##---------------------------------------------------------
FetchContent_Declare ( dsh
	URL "${DSH_URL}" 
)
FetchContent_GetProperties(dsh)
if(NOT dsh_POPULATED)
	FetchContent_Populate(dsh)
endif()

FetchContent_Declare ( sfml
	URL "${SFML_URL}"
)
FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
	FetchContent_Populate(sfml)
endif()

FetchContent_Declare ( json 
	URL                   "${JSON_URL}"
	DOWNLOAD_NO_PROGRESS  TRUE
	DOWNLOAD_NO_EXTRACT   TRUE
)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
	FetchContent_Populate(json)
endif()


## Set paths to dependencies
##---------------------------------------------------------
set (DSH_FOLDER "${dsh_SOURCE_DIR}")
set (SFML_FOLDER "${sfml_SOURCE_DIR}")
set (JSON_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/_deps/json-subbuild/json-populate-prefix/src")

### dshlibs
###--------------------------------------------------------
include_directories("${DSH_FOLDER}/include")
link_directories("${DSH_FOLDER}/lib")

### SFML
###--------------------------------------------------------
include_directories("${SFML_FOLDER}/include")
link_directories("${SFML_FOLDER}/lib")

### json
###--------------------------------------------------------
include_directories("${JSON_FOLDER}") # only header file, no library

# Source files
#----------------------------------------------------------
set (DGM_SOURCES
	src/Animation.cpp
	src/App.cpp
	src/Clip.cpp
	src/Collision.cpp
	src/Controller.cpp
	src/Conversion.cpp
	src/Objects.cpp
	src/Particle.cpp
	src/ParticleSystem.cpp
	src/ParticleSystemRenderer.cpp
	src/ResourceManager.cpp
	src/Tileset.cpp
	src/Window.cpp
)

set (DGM_HEADERS
	include/DGM/dgm.hpp
	include/DGM/classes/Animation.hpp
	include/DGM/classes/App.hpp
	include/DGM/classes/AppState.hpp
	include/DGM/classes/Buffer.hpp
	include/DGM/classes/Clip.hpp
	include/DGM/classes/Collision.hpp
	include/DGM/classes/Controller.hpp
	include/DGM/classes/Conversion.hpp
	include/DGM/classes/Error.hpp
	include/DGM/classes/Math.hpp
	include/DGM/classes/Objects.hpp
	include/DGM/classes/Particle.hpp
	include/DGM/classes/ParticleSystem.hpp
	include/DGM/classes/ParticleSystemRenderer.hpp
	include/DGM/classes/ResourceManager.hpp
	include/DGM/classes/Tileset.hpp
	include/DGM/classes/Time.hpp
	include/DGM/classes/Window.hpp
)

# Main target - DGM library
#----------------------------------------------------------
set ( DGM_RAW "${PROJECT_NAME}-raw" )
add_library( ${PROJECT_NAME} STATIC
	${DGM_SOURCES} ${DGM_HEADERS}
)

target_compile_features( ${PROJECT_NAME} 
	INTERFACE ${MY_CXX_COMPILE_FEATURES}
)

target_include_directories ( ${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

## Add dsh libraries directly to build
##---------------------------------------------------------
# NOTE: SFML is linked dynamically, dgm.hpp takes care of that
find_library(LIB_CFG_D config-d  NAMES config-d.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_D strings-d NAMES strings-d.lib HINTS "${DSH_FOLDER}/lib")
find_library(LIB_LVLD_D leveld-d NAMES leveld-d.lib HINTS "${DSH_FOLDER}/lib")

find_library(LIB_CFG_R config  NAMES config.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_R strings NAMES strings.lib HINTS "${DSH_FOLDER}/lib")
find_library(LIB_LVLD_R leveld NAMES leveld.lib HINTS "${DSH_FOLDER}/lib")

set(LIB_CFG optimized ${LIB_CFG_R} debug ${LIB_CFG_D})
set(LIB_STR optimized ${LIB_STR_R} debug ${LIB_STR_D})
set(LIB_LVLD optimized ${LIB_LVLD_R} debug ${LIB_LVLD_D})

target_link_libraries(${PROJECT_NAME} ${LIB_CFG} ${LIB_LVLD} ${LIB_STR})

## Bundle dsh libs with DGM
##---------------------------------------------------------
set ( DGM_RAW_D   "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}-d.lib" )
set ( DGM_RAW_R   "${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.lib" )
set ( RESULT_D    "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}-d.lib" )
set ( RESULT_R    "${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.lib" )

add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND
		lib.exe /OUT:$<$<CONFIG:debug>:${RESULT_D}>$<$<CONFIG:release>:${RESULT_R}> $<$<CONFIG:debug>:${DGM_RAW_D}>$<$<CONFIG:release>:${DGM_RAW_R}> $<$<CONFIG:debug>:${LIB_CFG_D}>$<$<CONFIG:release>:${LIB_CFG_R}> $<$<CONFIG:debug>:${LIB_LVLD_D}>$<$<CONFIG:release>:${LIB_LVLD_R}> $<$<CONFIG:debug>:${LIB_STR_D}>$<$<CONFIG:release>:${LIB_STR_R}>
	COMMENT "lib.exe /OUT:$<$<CONFIG:debug>:${RESULT_D}>$<$<CONFIG:release>:${RESULT_R}> $<$<CONFIG:debug>:${DGM_RAW_D}>$<$<CONFIG:release>:${DGM_RAW_R}> $<$<CONFIG:debug>:${DSH_LIBS_D}>$<$<CONFIG:release>:${DSH_LIBS_R}>"
)

# Doxygen
#----------------------------------------------------------
include ( FindDoxygen )
find_package( Doxygen )

configure_file (
	${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
	${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
)

add_custom_target ( doxygen-docs
	COMMAND
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY
		${CMAKE_CURRENT_BINARY_DIR}
)

# Examples
#----------------------------------------------------------
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

## Example 01
##---------------------------------------------------------
add_executable(Example01 WIN32 examples/example01.cpp)
target_link_libraries(Example01 ${PROJECT_NAME})

## Example 02
##---------------------------------------------------------
add_executable(Example02 WIN32 examples/example02.cpp)
target_link_libraries(Example02 ${PROJECT_NAME})

## Example 03
##---------------------------------------------------------
add_executable(Example03 WIN32 examples/example03.cpp)
target_link_libraries(Example03 ${PROJECT_NAME})

## Example 04
##---------------------------------------------------------
add_executable(Example04 WIN32 examples/example04.cpp)
target_link_libraries(Example04 ${PROJECT_NAME})

## Example 05
##---------------------------------------------------------
add_executable(Example05 WIN32 examples/example05.cpp)
target_link_libraries(Example05 ${PROJECT_NAME})

## Example 06
##---------------------------------------------------------
add_executable(Example06 WIN32 examples/example06.cpp)
target_link_libraries(Example06 ${PROJECT_NAME})

## Example 07
##---------------------------------------------------------
add_executable(Example07 WIN32 examples/example07.cpp)
target_link_libraries(Example07 ${PROJECT_NAME})

## Example 08
##---------------------------------------------------------
add_executable(Example08 WIN32 examples/example08.cpp)
target_link_libraries(Example08 ${PROJECT_NAME})

## Example 09
##---------------------------------------------------------
add_executable(Example09 WIN32 examples/example09.cpp)
target_link_libraries(Example09 ${PROJECT_NAME})

## Example 10
##---------------------------------------------------------
add_executable(Example10 WIN32 examples/example10.cpp)
target_link_libraries(Example10 ${PROJECT_NAME})

# Install directives
#----------------------------------------------------------

install (
	FILES        ${CMAKE_CURRENT_BINARY_DIR}/Release/dgm.lib
	DESTINATION  lib
)

install (
	FILES        ${CMAKE_CURRENT_BINARY_DIR}/Debug/dgm-d.lib
	DESTINATION  lib
)

install (
	DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/include/DGM
	DESTINATION  include
)

install (
	DIRECTORY    ${DSH_FOLDER}/include/
	DESTINATION  include
)

## Documentation
##---------------------------------------------------------
install (
	DIRECTORY    ${CMAKE_CURRENT_BINARY_DIR}/html
	DESTINATION  docs
)

install (
	FILES        ${CMAKE_CURRENT_SOURCE_DIR}/docs/quickstart.md
	DESTINATION  docs
)

# Packaging
#----------------------------------------------------------
set ( CPACK_GENERATOR "ZIP" )

set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ extension library for SFML" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md" )
set ( CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_FILE_NAME           "${PROJECT_NAME}-${PROJECT_VERSION}-vc15x64" )

include ( CPack )
