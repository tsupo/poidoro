echo off

REM batch file for distribution of "poidoro"
REM $Header: /comm/poidoro/dist.bat 1     09/05/14 4:46 tsupo $

chmod -R +w dist

rm -Rf dist\src

mkdir dist\src
mkdir dist\src\include
mkdir dist\src\lib

copy *.c dist\src
copy ..\blogolee\blogs\colors.c dist\src

copy Release\poidoro.exe dist
copy Release\xmlRPC.dll dist

copy ..\xmlRPC\xmlRPC.h dist\src\include
copy ..\xmlRPC\Release\xmlRPC.lib dist\src\lib
