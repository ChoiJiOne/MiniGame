@echo off

echo =============================
echo Start Generate Project Files
echo =============================

@REM CMake가 설치되었는지 확인합니다.
@echo off
where cmake > nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo CMake is installed on this system...
) else (
    echo CMake is not installed on this system...
    GOTO:EOF
)

@REM Visual Studio 버전입니다. 
SET vs=%1

@REM visual studio 버전을 검사합니다. 2019 혹은 2022입니다.
if "%vs%" == "vs2019" (
    SET visualstudio="Visual Studio 16 2019"
    echo Visual Studio version is "%vs%"...
) else if "%vs%" == "vs2022" (
    SET visualstudio="Visual Studio 17 2022"
    echo Visual Studio version is "%vs%"...
) else (
    echo Visual Studio version "%vs%" is illegal...
    GOTO:EOF
)

@REM 프로젝트 이름입니다.
SET project=%2

@REM 프로젝트 이름의 유효성을 검사합니다.
if %project% == "" (
    echo Project name is empty...
    GOTO:EOF
) else (
    echo Project name is "%project%"...
)

@REM 프로젝트 생성 후 Visual Studio를 실행할 지 여부입니다.
SET run=%3
if "%run%" == "on" (
    SET run="on"
) else if "%run%" == "On" (
    SET run="on"
) else if "%run%" == "off" (
    SET run="off"
) else if "%run%" == "Off" (
    SET run="off"
) else (
    SET run="off"
)
echo Switch %run% run Visual Studio Solution...

set solutionPath=%~dp0..\Solution

if not exist %solutionPath% (
    mkdir %solutionPath%
)

set currentPath=%~dp0

pushd %currentPath%
pushd %solutionPath%

cmake .. -G %visualstudio% -A "x64"

if %run% == "on" (
    start %project%.sln
)

popd
popd

echo =============================
echo Done Generate Project Files
echo =============================