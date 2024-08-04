@echo off

@ECHO OFF
where cmake > nul 2>&1
if %ERRORLEVEL% EQU 0 (
    ECHO CMake is installed on this system...
) else (
    ECHO CMake is not installed on this system...
    GOTO:EOF
)

SET visualstudio="Visual Studio 17 2022"
SET CURRENT_PATH=%~dp0
SET SOLUTION_PATH=%~dp0..\Solution

if not exist "%SOLUTION_PATH%" (
    mkdir "%SOLUTION_PATH%"
)

PUSHD "%CURRENT_PATH%"
PUSHD "%SOLUTION_PATH%"

cmake .. -G %visualstudio% -A "x64"

POPD
POPD