@ECHO OFF

ECHO =====================
ECHO Start Build Solution
ECHO =====================

where cmake > nul 2>&1
if %ERRORLEVEL% EQU 0 (
    ECHO CMake is installed on this system...
) else (
    ECHO CMake is not installed on this system...
    GOTO:EOF
)

SET SOLUTION_PATH=%~dp0..\..\Solution

if not exist "%SOLUTION_PATH%" (
    mkdir "%SOLUTION_PATH%"
)

SET MODE=%1

if "%MODE%" == "Debug" (
    ECHO Build "%MODE%" mode...
) else if "%MODE%" == "Release" (
    ECHO Build "%MODE%" mode...
) else if "%MODE%" == "RelWithDebInfo" (
    ECHO Build "%MODE%" mode...
) else if "%MODE%" == "MinSizeRel" (
    ECHO Build "%MODE%" mode...
) else (
    ECHO "%MODE%" is illegal mode...
    GOTO:EOF
)

SET CURRENT_PATH=%~dp0

PUSHD "%CURRENT_PATH%"
PUSHD "%SOLUTION_PATH%"

cmake --build . --config %mode% --clean-first --parallel  

POPD
POPD

ECHO =====================
ECHO Done Build Solution
ECHO =====================