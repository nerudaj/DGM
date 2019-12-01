# Quickstart Guide

Following guide assumes that you have project with following file structure:

 * root
	* CMakeLists.txt
	* Main.cpp
	* dgm/
		* lib/*
		* include/*

The contents of `CMakeLists.txt` and `Main.cpp` are presented as below.

## Minimal CMakeLists.txt

Following `CMakeLists.txt` file will automatically download SFML 2.5.1 and link it to your project. It will also link DGM, although you need to download and unpack it by yourself (see project file structure above).

```cmake
cmake_minimum_required ( VERSION 3.0 )

set (CMAKE_GENERATOR_PLATFORM "x64")

project ( MinimalExample )

# Download SFML
set ( SFML_VERSION "2.5.1" )
set ( SFML_URL "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )

include ( FetchContent )

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

# Link DGM
set ( DGM_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/dgm" )

find_library ( DGM_D NAMES dgm-d.lib HINTS ${DGM_FOLDER}/lib )
find_library ( DGM_R NAMES dgm.lib HINTS ${DGM_FOLDER}/lib )
set( DGM optimized ${DGM_R} debug ${DGM_D})

include_directories ( ${DGM_FOLDER}/include )
link_directories( ${DGM_FOLDER}/lib )

# Create target
add_executable ( ${PROJECT_NAME} Main.cpp )
target_link_libraries ( ${PROJECT_NAME} ${DGM} )
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