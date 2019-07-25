# DGM

DGM is C++ library meant for faster and more convenient 2D game development with popular library SFML. Among other features there is animation engine and particle system engine.

## Setting up

If you want to build the DGM from source you'll need to do this:

 * Install Visual Studio 2017
 * Download proper dependencies into 3rdParty folder. For more details refer to readme in that folder
 * Create a directory called `vsbuild` and run command `cmake ..` in it
 * Now you can fire up newly created solution file and start compiling

## Packaging

First off, check that changelog.txt reflects all changes made to the project, then determine new version and write this version to the `VERSION` file.

Next, run the `prepare-release.bat` and wait until the script finishes (there's a `pause` at the end of the script, you need to check for that.

Now, in the `RELEASE` folder you can find folders `DGM-<version>` and `DGM-Project-Template-<version>`. Zip each of these folders separately. These zips are your release.

*NOTE:* DGM is using something called dshlibs. Since it is stuff from my personal repo, headers of dshlibs are bundled with DGM and code is statically compiled *into* DGM itself. SFML, on the other hand is not mine and user have to download it separately.