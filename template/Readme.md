# DGM Project Template

This is a project template to quickly get you on track when building new games with DGM.

## How to use

 1. Run the `bootstrap.bat`. It will setup folder structure and copy appropriate files where they should be
    a. You can delete `bootstrap.bat` after this
 2. Go to `3rdParty` folder. Follow the steps outlined in the `Readme.md` placed there
 3. Modify CMakeLists.txt and set your own project name
 4. Create build folder for cmake and configure the project from there
 5. You're good to go!

## Releasing

After bootstrapping, one of the things that will appear is the `prepare-release.bat` script. You can use this for clean build of your project in the correct configuration and automated packaging. Running this script will:

 1. Delete your build directory
 2. Recreate your build directory and performing clean cmake configuration
 3. Build your project in Release setup
 4. Prepare release filestructure
 5. Copy binaries, libraries and assets to release file structure

For this to work, you either need to have cmake in path or have dsh suite. You also need to have VS 2017. The last thing you have to do is to modify variables declared in the top of the script to match some of your settings (name of the builddir, name of solution, name of project, etc).

If you consider the script to be useful, feel free to add whatever operations and files you fancy.