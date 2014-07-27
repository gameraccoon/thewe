@echo off

del ..\..\..\build\Windows\*.pdb
del ..\..\..\build\Windows\*.exp
del ..\..\..\build\Windows\*.lib
del ..\..\..\build\Windows\*.ilk
del ..\..\..\build\Windows\*.pdb

del *.sdf
del *.suo
del log.txt

rd /S /Q Debug
rd /S /Q Release
rd /S /Q ipch