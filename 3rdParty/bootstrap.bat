@echo off

echo Downloading SFML
cscript C:\tools\bin\wget.js "https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-32-bit.zip" sfml.zip
cscript C:\tools\bin\unzip.vbs sfml.zip
move SFML-2.5.1 SFML

echo Downloading dshlibs
cscript C:\tools\bin\wget.js "https://github.com/nerudaj/dsh/releases/download/dshlibs_v0.1.1/dshlibs-0.1.1.zip" dshlibs.zip
cscript C:\tools\bin\unzip.vbs dshlibs.zip
move dshlibs-0.1.1 dshlibs

del sfml.zip dshlibs.zip

cd ..
mkdir vsbuild\Debug
mkdir vsbuild\Release

echo Copying dlls
robocopy 3rdParty\SFML\bin vsbuild\Release sfml-audio-2.dll sfml-graphics-2.dll sfml-network-2.dll sfml-system-2.dll sfml-window-2.dll openal32.dll
robocopy 3rdParty\SFML\bin vsbuild\Debug sfml-audio-d-2.dll sfml-graphics-d-2.dll sfml-network-d-2.dll sfml-system-d-2.dll sfml-window-d-2.dll openal32.dll

@echo on