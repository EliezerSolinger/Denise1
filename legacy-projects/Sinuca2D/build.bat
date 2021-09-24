@echo off
cd build
cd bin
del libgrota3d.dll
del %1.exe
cd ..
mingw32-make sinuca
copy libgrota3d.dll bin
copy %1.exe bin
cd bin
%1 
cd ..\..\
