@ECHO off

ECHO ===================
ECHO Start Setup Project
ECHO ===================
ECHO.

SET NAME=%1
SET ROOT_PATH=%~dp0..\..\
SET PROJECT_PATH=%ROOT_PATH%%NAME%
SET LUA_PATH=%~dp0..\Bin\lua54.exe

if not defined name (
    ECHO Project name is empty!
    ECHO.
    ECHO ===================
    ECHO Done Setup Project
    ECHO ===================
    GOTO:EOF
)

if exist "%PROJECT_PATH%" (
    ECHO Already setup project!    
    ECHO.
    ECHO ===================
    ECHO Done Setup Project
    ECHO ===================
    GOTO:EOF
) else (
    mkdir "%PROJECT_PATH%"
    mkdir "%PROJECT_PATH%\Inc"
    mkdir "%PROJECT_PATH%\Src"
)

"%LUA_PATH%" "%~dp0\Setup.lua" "%NAME%" "%ROOT_PATH%" "%PROJECT_PATH%"

ECHO.
ECHO ===================
ECHO Done Setup Project
ECHO ===================