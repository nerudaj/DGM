cmake_minimum_required ( VERSION 3.19 )

project ( examples )

include ( ../cmake/utils.cmake )
copy_dlls_to_build_folders()

set ( ${DGM_LIB} libdgm )

include_directories ( ../lib/include )

# Targets
add_executable(01_polar_cartesian WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/01_polar_cartesian.cpp)
target_link_libraries(01_polar_cartesian libdgm )

add_executable(02_controller WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/02_controller.cpp)
target_link_libraries(02_controller libdgm )

add_executable(03_app_state WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/03_app_state.cpp)
target_link_libraries(03_app_state libdgm )

add_executable(04_particle_system WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/04_particle_system.cpp)
target_link_libraries(04_particle_system libdgm )

add_executable(05_game_fifteen WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/05_game_fifteen.cpp)
target_link_libraries(05_game_fifteen libdgm )

add_executable(06_animation WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/06_animation.cpp)
target_link_libraries(06_animation libdgm )

add_executable(07_collisions WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/07_collisions.cpp)
target_link_libraries(07_collisions libdgm )

add_executable(08_buffer_mesh WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/08_buffer_mesh.cpp)
target_link_libraries(08_buffer_mesh libdgm )

add_executable(09_resource_manager WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/09_resource_manager.cpp)
target_link_libraries(09_resource_manager libdgm )

add_executable(10_tileset WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/10_tileset.cpp)
target_link_libraries(10_tileset libdgm )

add_executable(11_advanced_particle_system WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/11_advanced_particle_system.cpp)
target_link_libraries(11_advanced_particle_system libdgm )

add_executable(12_simple_lights WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/12_simple_lights.cpp)
target_link_libraries(12_simple_lights libdgm )

add_executable(13_camera WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/13_camera.cpp)
target_link_libraries(13_camera libdgm )

# Install
install (
	FILES		 ${CMAKE_CURRENT_SOURCE_DIR}/Readme.md
	DESTINATION  examples
)

install (
	DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/resources
	DESTINATION  examples
)

install (
	DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/src
	DESTINATION  examples
)
