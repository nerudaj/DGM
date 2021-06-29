cmake_minimum_required ( VERSION 3.19 )

# Enforce C++ standards and compiler behaviour
include ( cmake/cpp.cmake )

# Project setup
project ( dgm )
include ( cmake/version.cmake )
string ( TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER )
enable_testing()

# Dependencies
include ( cmake/dependencies.cmake )
include ( cmake/utils.cmake )
copy_dlls_to_build_folders()

# Subprojects
add_subdirectory ( lib )
add_subdirectory ( docs )
add_subdirectory ( examples )

# Packaging
set ( CPACK_GENERATOR "ZIP" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Extension library for SFML" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md" )
set ( CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_FILE_NAME           "${PROJECT_NAME}-${PROJECT_VERSION}-windows-vc16-64-bit" )

include ( CPack )
