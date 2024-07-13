@echo off

ECHO ====================
ECHO Start Setup Project
ECHO ====================

python --version >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    ECHO Python is installed...
) ELSE (
    ECHO Python is not installed...
    ECHO ====================
    ECHO Done Setup Project
    ECHO ====================
    GOTO:EOF
)

SET PROJECT_NAME=%1
if "%PROJECT_NAME%" == "" (
    ECHO Project name is empty...
    ECHO ====================
    ECHO Done Setup Project
    ECHO ====================
    GOTO:EOF
) else if "%PROJECT_NAME%" == "GameMaker" (
    ECHO Project name "%PROJECT_NAME%" is invalid...
    ECHO ====================
    ECHO Done Setup Project
    ECHO ====================
    GOTO:EOF
) else (
    ECHO Project name is "%PROJECT_NAME%"...
)

if not exist "%~dp0\\Cache" (
    mkdir "%~dp0\\Cache"
)

if not exist "%PROJECT_NAME%" (
    mkdir "%PROJECT_NAME%"
)

if not exist "%PROJECT_NAME%\\Inc" (
    mkdir "%PROJECT_NAME%\\Inc"
)

if not exist "%PROJECT_NAME%\\Src" (
    mkdir "%PROJECT_NAME%\\Src"
)

if not exist "%PROJECT_NAME%\\Res" (
    mkdir "%PROJECT_NAME%\\Res"
)

SET OPTION=%2
if "%OPTION%" == "" (
    SET OPTION="NONE"
)

SET SETUP_SCRIPT_PATH=%~dp0GameMaker\Script\Setup.py
python "%SETUP_SCRIPT_PATH%" "%PROJECT_NAME%" "%OPTION%"

ECHO ====================
ECHO Done Setup Project
ECHO ====================