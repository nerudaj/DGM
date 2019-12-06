# DGM Project Template

This is a project template to quickly get you on track when building new games with DGM.

## Prerequisities

 * Visual Studio 15 (2017) or newer
 * CMake 3.14 or newer

## How to start

This project build system is governed by cmake. Simply type these commands to generate solution for Visual Studio:

```sh
mkdir build
cd build
cmake ..
```

This will produce solution file `build/Template.sln`, download all dependencies (DGM, SFML and TGUI by default), copy all DLLs to appropriate folders and link all dependencies.

You can launch the solution file, build it and launch it immediately.

## Releasing

For your convenience, this template supports packaging whole project using ZIP cpack generator. Unless you need to add new files/dependencies to `CMakeLists.txt`, the release process is pretty straightforward:

 1. Bump version number in file `version.cmake`
 2. Run `prepare-release.bat`
 3. Wait until folder `RELEASE` is produced. It should contain ZIP file with complete release of your application