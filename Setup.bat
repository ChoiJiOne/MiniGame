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

SET OPTION=%2
if "%OPTION%" == "" (
    SET OPTION="NONE"
)

SET SETUP_SCRIPT_PATH=%~dp0GameMaker\Script\Setup.py
python "%SETUP_SCRIPT_PATH%" "%PROJECT_NAME%" "%OPTION%"

ECHO ====================
ECHO Done Setup Project
ECHO ====================