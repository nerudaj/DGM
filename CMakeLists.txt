cmake_minimum_required(VERSION 3.4)
project (dgm)

set (NAME ${PROJECT_NAME})
set (SFML_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SFML")
set (DSH_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/dshlibs")

if (NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Where SFML is installed
include_directories("${SFML_FOLDER}/include")
link_directories("${SFML_FOLDER}/lib")

# Where dsh libraries are installed
include_directories("${DSH_FOLDER}/include")
link_directories("${DSH_FOLDER}/lib")

# DGM library
add_library(${NAME}
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
	src/TilesetRenderer.cpp
	src/Window.cpp
)

target_include_directories (${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add dsh libraries directly to build
# SFML is linked dynamically, dgm.hpp takes care of that
find_library(LIB_LOG_D logger-d  NAMES logger-d.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_CFG_D config-d  NAMES config-d.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_D strings-d NAMES strings-d.lib HINTS "${DSH_FOLDER}/lib")
message(${LIB_CFG_D})

find_library(LIB_LOG_R logger  NAMES logger.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_CFG_R config  NAMES config.lib  HINTS "${DSH_FOLDER}/lib")
find_library(LIB_STR_R strings NAMES strings.lib HINTS "${DSH_FOLDER}/lib")
message(${LIB_CFG_R})

set(LIB_LOG optimized ${LIB_LOG_R} debug ${LIB_LOG_D})
set(LIB_CFG optimized ${LIB_CFG_R} debug ${LIB_CFG_D})
set(LIB_STR optimized ${LIB_STR_R} debug ${LIB_STR_D})

target_link_libraries(${NAME} ${LIB_LOG} ${LIB_CFG} ${LIB_STR})

# Sandbox target
add_executable(Sandbox WIN32 tests/sandbox.cpp)
target_include_directories (Sandbox PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Sandbox ${NAME})