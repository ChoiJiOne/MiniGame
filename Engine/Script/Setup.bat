@echo off

echo ===================
echo Start Setup Project
echo ===================
echo.

SET name=%1
SET root=%~dp0..\..\
SET project=%root%%name%
SET lua=%~dp0..\Bin\lua54.exe

if not defined name (
    echo Project name is empty!
    echo.
    echo ===================
    echo Done Setup Project
    echo ===================
    GOTO:EOF
)

if exist %project% (
    echo Already setup project!    
    echo.
    echo ===================
    echo Done Setup Project
    echo ===================
    GOTO:EOF
) else (
    mkdir %project%
    mkdir %project%\\Inc
    mkdir %project%\\Src
)

%lua% %~dp0Setup.lua %name% %root% %project%

echo.
echo ===================
echo Done Setup Project
echo ===================