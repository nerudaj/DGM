cmake_minimum_required ( VERSION 3.10 )

## Project setup
project ( Template
	LANGUAGES CXX
)

## C++ standards
set ( CMAKE_CXX_STANDARD           17 )
set ( CMAKE_CXX_STANDARD_REQUIRED  ON )
set ( CMAKE_CXX_EXTENSIONS         OFF )
set ( MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums )

## Dependencies
set ( SFML_FOLDER  "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SFML" )
set ( DGM_FOLDER   "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/DGM" )

include_directories(
	"${SFML_FOLDER}/include"
	"${DGM_FOLDER}/include"
)

link_directories(
	"${SFML_FOLDER}/lib"
	"${DGM_FOLDER}/lib"
)

### Find libraries
# NOTE: SFML is linked dynamically from DGM
find_library ( LIB_DGM_D dgm-d
	NAMES
		dgm-d.lib
	HINTS
		"${DGM_FOLDER}/lib"
)

find_library ( LIB_DGM_R dgm
	NAMES
		dgm.lib
	HINTS
		"${DGM_FOLDER}/lib"
)

set ( LIB_DGM
	optimized
		${LIB_DGM_R}
	debug
		${LIB_DGM_D}
)

set ( SOURCE_FILES
	src/Main.cpp
	src/AppStateMainMenu.cpp
	src/AppStateMenuOptions.cpp
	src/AppStateIngame.cpp
)

set ( HEADER_FILES
	src/AppStateMainMenu.hpp
	src/AppStateMenuOptions.hpp
	src/AppStateIngame.hpp
	src/Gui.hpp
)

add_executable ( ${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
)
target_link_libraries( ${PROJECT_NAME} ${LIB_DGM} )
target_compile_features( ${PROJECT_NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES} )