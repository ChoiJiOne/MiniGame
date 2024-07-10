@echo off

ECHO ====================
ECHO Start Setup Project
ECHO ====================

python --version >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    ECHO Python is installed...
) ELSE (
    ECHO Python is not installed...
    GOTO:EOF
)

ECHO ====================
ECHO Done Setup Project
ECHO ====================