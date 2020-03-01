## Example 01
##---------------------------------------------------------
add_executable(01_polar_cartesian WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/01_polar_cartesian.cpp)
target_link_libraries(01_polar_cartesian ${DGM_LIB})

## Example 02
##---------------------------------------------------------
add_executable(02_controller WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/02_controller.cpp)
target_link_libraries(02_controller ${DGM_LIB})

## Example 03
##---------------------------------------------------------
add_executable(03_app_state WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/03_app_state.cpp)
target_link_libraries(03_app_state ${DGM_LIB})

## Example 04
##---------------------------------------------------------
add_executable(04_particle_system WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/04_particle_system.cpp)
target_link_libraries(04_particle_system ${DGM_LIB})

## Example 05
##---------------------------------------------------------
add_executable(05_game_fifteen WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/05_game_fifteen.cpp)
target_link_libraries(05_game_fifteen ${DGM_LIB})

## Example 06
##---------------------------------------------------------
add_executable(06_animation WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/06_animation.cpp)
target_link_libraries(06_animation ${DGM_LIB})

## Example 07
##---------------------------------------------------------
add_executable(07_collisions WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/07_collisions.cpp)
target_link_libraries(07_collisions ${DGM_LIB})

## Example 08
##---------------------------------------------------------
add_executable(08_buffer_mesh WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/08_buffer_mesh.cpp)
target_link_libraries(08_buffer_mesh ${DGM_LIB})

## Example 09
##---------------------------------------------------------
add_executable(09_resource_manager WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/09_resource_manager.cpp)
target_link_libraries(09_resource_manager ${DGM_LIB})

## Example 10
##---------------------------------------------------------
add_executable(10_tileset WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/10_tileset.cpp)
target_link_libraries(10_tileset ${DGM_LIB})

## Example 11
##---------------------------------------------------------
add_executable(11_advanced_particle_system WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/src/11_advanced_particle_system.cpp)
target_link_libraries(11_advanced_particle_system ${DGM_LIB})
