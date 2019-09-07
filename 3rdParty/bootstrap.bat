@echo off

echo Downloading dshlibs
cscript C:\tools\bin\wget.js "https://github.com/nerudaj/dsh/releases/download/v0.6.2/dshlibs-0.6.2-vs2019-x64.zip" dshlibs.zip
cscript C:\tools\bin\unzip.vbs dshlibs.zip
move dshlibs-0.6.2-vs2019-x64 dshlibs

echo Downloading SFML
cscript C:\tools\bin\wget.js "https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-32-bit.zip" sfml.zip
cscript C:\tools\bin\unzip.vbs sfml.zip
move SFML-2.5.1 SFML

del sfml.zip dshlibs.zip

echo Downloading nlohmann's json
cscript C:\tools\bin\wget.js "https://github.com/nlohmann/json/releases/download/v3.6.1/json.hpp" json.hpp
mkdir json
move json.hpp json

cd ..
mkdir vsbuild\Debug
mkdir vsbuild\Release

echo Copying dlls
robocopy 3rdParty\SFML\bin vsbuild\Release sfml-audio-2.dll sfml-graphics-2.dll sfml-network-2.dll sfml-system-2.dll sfml-window-2.dll openal32.dll
robocopy 3rdParty\SFML\bin vsbuild\Debug sfml-audio-d-2.dll sfml-graphics-d-2.dll sfml-network-d-2.dll sfml-system-d-2.dll sfml-window-d-2.dll openal32.dll

@echo on