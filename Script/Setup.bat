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
) else if "%PROJECT_NAME%" == "MiniGame" (
    ECHO Project name "%PROJECT_NAME%" is invalid...
    ECHO ====================
    ECHO Done Setup Project
    ECHO ====================
    GOTO:EOF
) else (
    ECHO Project name is "%PROJECT_NAME%"...
)

SET CURRENT_PATH=%~dp0

PUSHD "%CURRENT_PATH%"
PUSHD "%~dp0..\\..\\"

if not exist "%cd%\\Cache" (
    mkdir "%cd%\\Cache"
)

if not exist "%cd%\\%PROJECT_NAME%" (
    mkdir "%cd%\\%PROJECT_NAME%"
)

if not exist "%cd%\\%PROJECT_NAME%\\Inc" (
    mkdir "%cd%\\%PROJECT_NAME%\\Inc"
)

if not exist "%cd%\\%PROJECT_NAME%\\Src" (
    mkdir "%cd%\\%PROJECT_NAME%\\Src"
)

if not exist "%cd%\\%PROJECT_NAME%\\Res" (
    mkdir "%cd%\\%PROJECT_NAME%\\Res"
)

SET SETUP_SCRIPT_PATH=%~dp0Setup.py
python "%SETUP_SCRIPT_PATH%" "%PROJECT_NAME%"

POPD
POPD

ECHO ====================
ECHO Done Setup Project
ECHO ====================