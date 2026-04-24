@echo off
set PATH=C:\msys64\mingw64\bin;C:\msys64\mingw64\share\qt6\bin;C:\msys64\usr\bin;%PATH%
echo Starting server...
start "" /MIN "C:\MyProject\Server\release\Server.exe"
timeout /t 1 /nobreak >nul
echo Starting client...
start "" /WAIT "C:\MyProject\Client\release\Client.exe"
echo Closing server...
taskkill /IM Server.exe /F >nul 2>&1
echo Done.
