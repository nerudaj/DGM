mkdir 3rdParty
mkdir src
mkdir audio
mkdir audio\sounds
mkdir audio\music

cd stuff

move CMakeLists.txt ..
move *.hpp ..\src
move *.cpp ..\src
move graphics ..
move VERSION ..
move prepare-release.bat ..

move 3rdparty-readme.md ..\3rdParty\Readme.md

cd ..

mkdir graphics\textures
mkdir graphics\configs

rmdir stuff
del /y bootstrap.bat
