# Quickstart Guide

Following guide assumes that you have project with following file structure:

```
root/
 + CMakeLists.txt
 + src/
	+ Main.cpp
```

The contents of `CMakeLists.txt` and `Main.cpp` are presented as below.

## Minimal CMakeLists.txt

Following `CMakeLists.txt` file will automatically download SFML 2.5.1 and DGM 1.7.1 and link them to the project. Don't forget to update versions as needed.

```cmake
cmake_minimum_required ( VERSION 3.0 )

set (CMAKE_GENERATOR_PLATFORM "x64")

project ( MinimalExample )

# Declare dependencies
# SFML
set ( SFML_VERSION "2.5.1" )
set ( SFML_URL "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )

# DGM
set ( DGM_VERSION "1.7.1" )
set ( DGM_URL "https://github.com/nerudaj/DGM/releases/download/v${DGM_VERSION}/dgm-${DGM_VERSION}-windows-vc15-64-bit.zip" )


# Fetch dependencies

include ( FetchContent )

# Fetch and link SFML
FetchContent_Declare ( sfml
	URL "${SFML_URL}"
)
FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
	FetchContent_Populate(sfml)
endif()

set (SFML_FOLDER "${sfml_SOURCE_DIR}")
include_directories("${SFML_FOLDER}/include")
link_directories("${SFML_FOLDER}/lib")

# Fetch and link DGM
FetchContent_Declare ( dgm
	URL "${DGM_URL}"
)
FetchContent_GetProperties(dgm)
if(NOT dgm_POPULATED)
	FetchContent_Populate(dgm)
endif()

set (DGM_FOLDER "${dgm_SOURCE_DIR}")
include_directories("${DGM_FOLDER}/include")
link_directories("${DGM_FOLDER}/lib")

find_library ( LIB_DGM_D dgm-d NAMES dgm-d.lib HINTS "${DGM_FOLDER}/lib" )
find_library ( LIB_DGM_R dgm   NAMES dgm.lib   HINTS "${DGM_FOLDER}/lib" )

set ( LIB_DGM optimized ${LIB_DGM_R} debug ${LIB_DGM_D} )

# Create target
add_executable ( ${PROJECT_NAME} src/Main.cpp )
target_link_libraries ( ${PROJECT_NAME} ${LIB_DGM} )
```

## Main.cpp

Following program will open up a window which will display nothing and will close upon hitting Escape.

```c++
#include <DGM/DGM.hpp>

int main() {
	dgm::Window window({1280, 720}, "Example Window", false);

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		// Logic

		// Draw
		window.beginDraw();

		// Put draw calls here

		window.endDraw();
	}
}
```

## Building the project

With project set up as such, use command `mkdir vsbuild && cd vsbuild && cmake ..` to generate project file `MinimalExample.sln` in `vsbuild` folder.

You will find SFML's *.dll files under `vsbuild/_deps/sfml-src/bin`.

# Project Template

You can also use the project template that comes with every [release of DGM](https://github.com/nerudaj/DGM/releases). The template is complete with main menu, options that include fullscreen toggle and resolution changes and it also links TGUI library.