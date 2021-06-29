@echo off

set BUILDDIR=vsbuild

echo Phase 1 - Purging
rd /s /q %BUILDDIR%
rd /s /q project-template\%BUILDDIR%

echo Phase 2 - Configuring
mkdir %BUILDDIR%
cd %BUILDDIR%
cmake.exe ..
cd ..

echo Phase 3 - Building
rem Source path to devenv
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

cd %BUILDDIR%
devenv dgm.sln /Build Debug
devenv dgm.sln /Build Release
devenv dgm.sln /Build Release /Project doxygen-docs
cpack
cd ..

echo Phase 4 - Finalizing
mkdir RELEASE
move %BUILDDIR%\*.zip RELEASE

echo Done!
pause

@echo on