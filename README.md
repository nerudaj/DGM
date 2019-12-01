# DGM

DGM is C++ library meant for faster and more convenient 2D game development with popular library SFML. Among other features there is animation engine and particle system engine.

## Building from source

### Prerequisities

 * Visual Studio version 15 (2017 and above)
 * cmake 3.16
 * doxygen

### Manual build

```
mkdir vsbuild
cd vsbuild
cmake ..
```

This will generate project file `dgm.sln` which you can boot up and compile. All dependencies are automatically downloaded during configure step. Since you will probably need SFML's dlls, you can find them under `vsbuild/_deps/sfml-src/bin`.

### Automated build

As long as you have [dsh suite](https://github.com/nerudaj/dsh) installed (or you have cmake and doxygen in your PATH), you can run `prepare-release.bat` script. It will build everything - debug version, release version and documentation and package it.

Results of this script will be placed into `RELEASE/dgm-<version>-vc15x64.zip` file. This archive is deployment ready.

## Creating new release

When creating new release, follow these steps:

 1. Review `changelog.txt`. Check whether it lists all relevant changes for the new version. Pick a new version number and log it to changelog.
 2. Open `version.cmake` and write down new version number.
 3. Run `prepare-release.bat` and wait until new release is produced.

## Bumping dependencies

Edit file `dependencies.cmake` and update version numbers in appropriate variables. Also check that `*_URL` variables are valid.

### Using DGM

When looking for tutorials for this library please refer to:

 * Examples in `examples` folder
 * Generated html docs from doxygen (bundled with each release)
 * `quickstart.md` in `docs` folder (also bundled with each release)