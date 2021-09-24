@echo off
cd build
cd bin
del libgrota3d.dll
del %1.exe
cd ..
mingw32-make core/fast grota3d/fast %1/fast
copy libgrota3d.dll bin
copy %1.exe bin
cd bin
%1 "C:\Users\alguem_me_mata_pfv\Documents\gitclones\Grota3D\build\bin\obj\alb1\alb1.obj"
cd ..\..\
