# DGM

DGM is C++ library meant for faster and more convenient 2D game development with popular library SFML. Among other features there is animation engine and particle system engine.

## Setting up

If you want to build the DGM from source you'll need to do this:

 * Install Visual Studio 2017
 * Download proper dependencies into 3rdParty folder. For more details refer to readme in that folder
 * Create a directory called `vsbuild` and run command `cmake ..` in it
 * Now you can fire up newly created solution file and start compiling

## Packaging

Packaging is done in three stages:

 1) Build DGM in Debug mode
 2) Build DGM in Release mode
 3) Run `prepare-release.bat` from root folder

This will create folder DGM-<version_number> which you can then zip and distribute!

*NOTE:* DGM is using something called dshlibs. Since it is stuff from my personal repo, headers of dshlibs are bundled with DGM and code is statically compiled *into* DGM itself. SFML, on the other hand is not mine and user have to download it separately.