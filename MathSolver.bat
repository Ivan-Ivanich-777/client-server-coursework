@echo off
title Math Solver
echo Starting Math Solver...
set PATH=C:\msys64\mingw64\bin;C:\msys64\mingw64\share\qt6\bin;C:\msys64\usr\bin;%PATH%
start "" /MIN "C:\MyProject\Server\release\Server.exe"
timeout /t 2 /nobreak >nul
start "" "C:\MyProject\Client\release\Client.exe"
