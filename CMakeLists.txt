cmake_minimum_required(VERSION 3.10)
project (dgm)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)

set (NAME ${PROJECT_NAME})
set (SFML_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SFML")
set (DSH_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/dshlibs")
set (JSON_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/json")

if (NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Where SFML is installed
include_directories("${SFML_FOLDER}/include")
link_directories("${SFML_FOLDER}/lib")

# Where dsh libraries are installed
include_directories("${DSH_FOLDER}/include")
link_directories("${DSH_FOLDER}/lib")

# Where json library is installed
include_directories("${JSON_FOLDER}") # only header file, no library

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

# DGM library
add_library(${NAME} ${DGM_SOURCES} ${DGM_HEADERS})
target_compile_features(${NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES})

target_include_directories (${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add dsh libraries directly to build
# SFML is linked dynamically, dgm.hpp takes care of that
find_library(LIB_LOG_D logger-d  NAMES logger-d.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_CFG_D config-d  NAMES config-d.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_D strings-d NAMES strings-d.lib HINTS "${DSH_FOLDER}/lib")
find_library(LIB_LVLD_D leveld-d NAMES leveld-d.lib HINTS "${DSH_FOLDER}/lib")
message(${LIB_CFG_D})

find_library(LIB_LOG_R logger  NAMES logger.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_CFG_R config  NAMES config.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_R strings NAMES strings.lib HINTS "${DSH_FOLDER}/lib")
find_library(LIB_LVLD_R leveld NAMES leveld.lib HINTS "${DSH_FOLDER}/lib")
message(${LIB_CFG_R})

set(LIB_LOG optimized ${LIB_LOG_R} debug ${LIB_LOG_D})
set(LIB_CFG optimized ${LIB_CFG_R} debug ${LIB_CFG_D})
set(LIB_STR optimized ${LIB_STR_R} debug ${LIB_STR_D})
set(LIB_LVLD optimized ${LIB_LVLD_R} debug ${LIB_LVLD_D})

target_link_libraries(${NAME} ${LIB_LOG} ${LIB_CFG} ${LIB_STR} ${LIB_LVLD})

# Example targets aka compile tests
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

## Example 01
add_executable(Example01 WIN32 examples/example01.cpp)
target_link_libraries(Example01 ${NAME})

## Example 02
add_executable(Example02 WIN32 examples/example02.cpp)
target_link_libraries(Example02 ${NAME})

## Example 03
add_executable(Example03 WIN32 examples/example03.cpp)
target_link_libraries(Example03 ${NAME})

## Example 04
add_executable(Example04 WIN32 examples/example04.cpp)
target_link_libraries(Example04 ${NAME})

## Example 05
add_executable(Example05 WIN32 examples/example05.cpp)
target_link_libraries(Example05 ${NAME})

## Example 06
add_executable(Example06 WIN32 examples/example06.cpp)
target_link_libraries(Example06 ${NAME})
