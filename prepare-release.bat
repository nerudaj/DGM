rem Get path to cmake
call /tools/doomsh.cmd

@echo off

echo Phase 1 - Purging
rd /s /q vsbuild

echo Phase 2 - Configuring
mkdir vsbuild
cd vsbuild
cmake.exe ..
cd ..

rem Silently copy dlls
robocopy 3rdParty\SFML\bin vsbuild\Release sfml-audio-2.dll sfml-graphics-2.dll sfml-network-2.dll sfml-system-2.dll sfml-window-2.dll openal32.dll
robocopy 3rdParty\SFML\bin vsbuild\Debug sfml-audio-d-2.dll sfml-graphics-d-2.dll sfml-network-d-2.dll sfml-system-d-2.dll sfml-window-d-2.dll openal32.dll

echo Phase 3 - Building
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
cd vsbuild
devenv dgm.sln /Build Debug
devenv dgm.sln /Build Release
cd ..

echo Phase 4 - Release filesys
set /p version=<VERSION
mkdir RELEASE
mkdir RELEASE\DGM-%version%
mkdir RELEASE\DGM-%version%\include
mkdir RELEASE\DGM-%version%\lib

echo Phase 5 - Copying data
copy changelog.txt RELEASE\DGM-%version%
lib.exe /OUT:RELEASE\DGM-%version%\lib\dgm-d.lib vsbuild\Debug\dgm-d.lib 3rdParty\dshlibs\lib\config-d.lib 3rdParty\dshlibs\lib\leveld-d.lib 3rdParty\dshlibs\lib\strings-d.lib
lib.exe /OUT:RELEASE\DGM-%version%\lib\dgm.lib vsbuild\Release\dgm.lib 3rdParty\dshlibs\lib\config.lib 3rdParty\dshlibs\lib\leveld.lib 3rdParty\dshlibs\lib\strings.lib
robocopy /S include RELEASE\DGM-%version%\include
robocopy 3rdParty\dshlibs\include RELEASE\DGM-%version%\include Config.hpp Args.hpp Item.hpp Csv.hpp Ini.hpp Strings.hpp LevelD.hpp

echo Phase 6 - Project template
mkdir RELEASE\DGM-Project-Template-%version%
robocopy /S template RELEASE\DGM-Project-Template-%version%

rem Cleanup auxiliary template files
cd RELEASE\DGM-Project-Template-%version%
rmdir /s /q vsbuild
rmdir /s /q 3rdParty/TGUI
rmdir /s /q 3rdParty/DGM
rmdir /s /q 3rdParty/SFML
rmdir /s /q RELEASE
del app.ini
cd ../..

echo Phase 7 - Generating doxydocs
cd docs
doxygen
move html ..\RELEASE\DGM-%version%\docs
cd ..

echo Done!

cd RELEASE
move DGM-%version% DGM-%version%-vs2019-x64

@echo on