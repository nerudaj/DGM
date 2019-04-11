cmake_minimum_required(VERSION 3.4)
project (dgm)

set (NAME ${PROJECT_NAME})

if (NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Where SFML is installed
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SFML/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SFML/lib")

# Where dsh libraries are installed
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/lib")

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
find_library(LIB_LOG logger  NAMES logger.lib  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/lib")
find_library(LIB_CFG config  NAMES config.lib  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/lib")
find_library(LIB_STR strings NAMES strings.lib HINTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/lib")
target_link_libraries(${NAME} ${LIB_LOG} ${LIB_CFG} ${LIB_STR})

# Sandbox target
add_executable(Sandbox WIN32 tests/sandbox.cpp)
target_include_directories (Sandbox PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Sandbox ${NAME})