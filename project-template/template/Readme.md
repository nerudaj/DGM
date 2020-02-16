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

## How to work with this template

There are three important components to this project:

 * AppStateMainMenu - This is the screen you are greeted with. It contains button for launching the game, accessing options and exiting the application. You can change the application title here.
 * AppStateOptions - This screen allows you to set volume of sound, resolution and toggle fullscreen. Add more options as needed.
 * AppStateIngame - Top component of the game itself. The class contains some members to showcase how some components can be retrieved and used. While the `Actor` subclass is only there to showcase animation, the `Soundbox` class might be useful for you.

If you are new to this template, I recommend to build it, launch it and just play with it to see all what it can do, then consult the code.

If you are familiar with it, simply purge the Actor from AppStateIngame and start developing!