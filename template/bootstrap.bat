mkdir 3rdParty
mkdir src
mkdir audio
mkdir audio\sounds
mkdir audio\music
mkdir graphics\textures
mkdir graphics\configs
mkdir graphics\fonts

cd stuff

move CMakeLists.txt ..
move *.hpp ..\src
move *.cpp ..\src
move cruft.ttf ..\graphics\fonts
move VERSION ..
move prepare-release.bat ..
move copy-dlls.bat ..

move 3rdparty-readme.md ..\3rdParty\Readme.md

cd ..

rmdir stuff
del /y bootstrap.bat
