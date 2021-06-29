cmake_minimum_required ( VERSION 3.19 )

project ( libdgm )

set ( HEADERS
	include/DGM/dgm.hpp
	include/DGM/classes/Animation.hpp
	include/DGM/classes/App.hpp
	include/DGM/classes/AppState.hpp
	include/DGM/classes/Buffer.hpp
	include/DGM/classes/Camera.hpp
	include/DGM/classes/Clip.hpp
	include/DGM/classes/Collision.hpp
	include/DGM/classes/Controller.hpp
	include/DGM/classes/Conversion.hpp
	include/DGM/classes/Error.hpp
	include/DGM/classes/Math.hpp
	include/DGM/classes/NavMesh.hpp
	include/DGM/classes/Objects.hpp
	include/DGM/classes/Particle.hpp
	include/DGM/classes/ParticleSystem.hpp
	include/DGM/classes/ParticleSystemRenderer.hpp
	include/DGM/classes/Path.hpp
	include/DGM/classes/ResourceManager.hpp
	include/DGM/classes/Shell.hpp
	include/DGM/classes/TileMap.hpp
	include/DGM/classes/Time.hpp
	include/DGM/classes/Window.hpp
)

set ( SOURCES
	src/Animation.cpp
	src/App.cpp
	src/Camera.cpp
	src/Clip.cpp
	src/Collision.cpp
	src/Controller.cpp
	src/Conversion.cpp
	src/NavMesh.cpp
	src/Objects.cpp
	src/Particle.cpp
	src/ParticleSystem.cpp
	src/ParticleSystemRenderer.cpp
	src/Path.cpp
	src/ResourceManager.cpp
	src/Shell.cpp
	src/TileMap.cpp
	src/Window.cpp
)

# Not contributing to build process, this is here to enable Intellisense
include_directories ( include )

# Setup target
add_library ( ${PROJECT_NAME}
	STATIC ${HEADERS} ${SOURCES}
)
target_include_directories ( ${PROJECT_NAME}
	PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
	PUBLIC  "${SFML_FOLDER}/include"
	PUBLIC  "${DSH_FOLDER}/include"
	PRIVATE "${JSON_FOLDER}"
)
target_compile_features ( ${PROJECT_NAME}
	INTERFACE ${MY_CXX_COMPILE_FEATURES}
)
target_link_libraries ( ${PROJECT_NAME} ${LIB_CFG} ${LIB_LVLD} ${LIB_STR} )

# Bundle dsh libs with DGM
set ( DGM_RAW_D   "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}-d.lib" )
set ( DGM_RAW_R   "${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.lib" )
set ( RESULT_D    "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}-d.lib" )
set ( RESULT_R    "${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.lib" )

# This glues static libraries of dsh into static library of DGM
# it respects debug/release versions
add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND
		lib.exe /OUT:$<$<CONFIG:debug>:${RESULT_D}>$<$<CONFIG:release>:${RESULT_R}> $<$<CONFIG:debug>:${DGM_RAW_D}>$<$<CONFIG:release>:${DGM_RAW_R}> $<$<CONFIG:debug>:${LIB_CFG_D}>$<$<CONFIG:release>:${LIB_CFG_R}> $<$<CONFIG:debug>:${LIB_LVLD_D}>$<$<CONFIG:release>:${LIB_LVLD_R}> $<$<CONFIG:debug>:${LIB_STR_D}>$<$<CONFIG:release>:${LIB_STR_R}>
	COMMENT "lib.exe /OUT:$<$<CONFIG:debug>:${RESULT_D}>$<$<CONFIG:release>:${RESULT_R}> $<$<CONFIG:debug>:${DGM_RAW_D}>$<$<CONFIG:release>:${DGM_RAW_R}> $<$<CONFIG:debug>:${DSH_LIBS_D}>$<$<CONFIG:release>:${DSH_LIBS_R}>"
)

# Testing
add_subdirectory ( tests )

# Instalation
install (
	FILES        ${CMAKE_CURRENT_BINARY_DIR}/Release/libdgm.lib
	DESTINATION  lib
)

install (
	FILES        ${CMAKE_CURRENT_BINARY_DIR}/Debug/libdgm-d.lib
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
