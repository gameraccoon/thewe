@echo off

del ..\..\..\release\Windows\*.pdb
del ..\..\..\release\Windows\*.exp
del ..\..\..\release\Windows\*.lib
del ..\..\..\release\Windows\*.ilk
del ..\..\..\release\Windows\*.pdb

del *.sdf
del *.suo
del log.txt

rd /S /Q Debug
rd /S /Q Release
rd /S /Q ipch