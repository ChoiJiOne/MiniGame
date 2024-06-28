@ECHO OFF

ECHO =====================
ECHO Start Build Solution
ECHO =====================

@REM CMake가 설치되었는지 확인합니다.
where cmake > nul 2>&1
if %ERRORLEVEL% EQU 0 (
    ECHO CMake is installed on this system...
) else (
    ECHO CMake is not installed on this system...
    GOTO:EOF
)

SET SOLUTION_PATH=%~dp0..\..\Solution

@REM 프로젝트가 존재하지 않으면 프로젝트를 생성합니다.
if not exist "%SOLUTION_PATH%" (
    mkdir "%SOLUTION_PATH%"
)

@REM 빌드할 모드입니다.
SET MODE=%1

@REM 빌드 모드의 유효성을 검사합니다.
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