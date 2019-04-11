@echo off

set /p version=<VERSION

echo Making release folders
mkdir DGM-%version%\include
mkdir DGM-%version%\lib
# TODO: docs

echo Copying data
copy vsbuild\Debug\dgm-d.lib DGM-%version%\lib
copy vsbuild\Release\dgm.lib DGM-%version%\lib
robocopy /S include DGM-%version%\include
robocopy 3rdParty\dshlibs\include DGM-%version%\include Config.hpp Args.hpp Item.hpp Csv.hpp Ini.hpp Logger.hpp Strings.hpp

@echo on