cmake_minimum_required ( VERSION 3.14 )

# Project setup
#----------------------------------------------------------
project ( "DGM-Project-Template" NONE )

include ( ../version.cmake )
include ( ../dependencies.cmake )

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/inputs/CMakeLists.txt.in
    ${CMAKE_CURRENT_SOURCE_DIR}/template/CMakeLists.txt
)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/inputs/dependencies.cmake.in
    ${CMAKE_CURRENT_SOURCE_DIR}/template/dependencies.cmake
)

# Installation
#----------------------------------------------------------
install (
    DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/sources/
    DESTINATION  .
)

# Packaging
#----------------------------------------------------------
set ( CPACK_GENERATOR "ZIP" )
set ( CPACK_CMAKE_GENERATOR "Ninja" )

set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ extension library for SFML" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/template/Readme.md" )
set ( CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_FILE_NAME           "${PROJECT_NAME}-${PROJECT_VERSION}-vc15x64" )

include ( CPack )
