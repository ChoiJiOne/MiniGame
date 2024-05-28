@echo off

echo =====================
echo Start Build Solution
echo =====================

@REM CMake가 설치되었는지 확인합니다.
@echo off
where cmake > nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo CMake is installed on this system...
) else (
    echo CMake is not installed on this system...
    GOTO:EOF
)

set solutionPath=%~dp0..\Solution

@REM 프로젝트가 존재하지 않으면 프로젝트를 생성합니다.
if not exist %solutionPath% (
    mkdir %solutionPath%
)

@REM 빌드할 모드입니다.
SET mode=%1

@REM 빌드 모드의 유효성을 검사합니다.
if "%mode%" == "Debug" (
    echo Build "%mode%" mode...
) else if "%mode%" == "Release" (
    echo Build "%mode%" mode...
) else if "%mode%" == "RelWithDebInfo" (
    echo Build "%mode%" mode...
) else if "%mode%" == "MinSizeRel" (
    echo Build "%mode%" mode...
) else (
    echo "%mode%" is illegal mode...
    GOTO:EOF
)

set currentPath=%~dp0

pushd "%currentPath%"
pushd "%solutionPath%"

cmake --build . --config %mode% --clean-first --parallel  

popd
popd

echo =====================
echo Done Build Solution
echo =====================