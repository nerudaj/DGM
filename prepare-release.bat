# Get path to cmake
call /tools/doomsh.cmd

@echo off

echo Phase 1 - Purging
rd /s /q vsbuild

echo Phase 2 - Configuring
mkdir vsbuild
cd vsbuild
cmake.exe ..
cd ..

echo Phase 3 - Building
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
cd vsbuild
devenv dgm.sln /Build Debug
devenv dgm.sln /Build Release
cd ..

echo Phase 4 - Release filesys
set /p version=<VERSION
mkdir RELEASE
mkdir RELEASE\DGM-%version%\include
mkdir RELEASE\DGM-%version%\lib

echo Phase 5 - Copying data
copy changelog.txt RELEASE\DGM-%version%
copy vsbuild\Debug\dgm-d.lib RELEASE\DGM-%version%\lib
copy vsbuild\Release\dgm.lib RELEASE\DGM-%version%\lib
robocopy /S include RELEASE\DGM-%version%\include
robocopy 3rdParty\dshlibs\include DGM-%version%\include Config.hpp Args.hpp Item.hpp Csv.hpp Ini.hpp Logger.hpp Strings.hpp

echo Phase 6 - Project template
mkdir RELEASE\DGM-Project-Template-%version%
robocopy /S template RELEASE\DGM-Project-Template-%version%

echo Done!

@echo on