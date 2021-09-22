@echo off
cd build
cd bin
del denise1.exe
cd ..
mingw32-make
copy denise1.exe bin
cd bin
denise1 
cd ..\..\
