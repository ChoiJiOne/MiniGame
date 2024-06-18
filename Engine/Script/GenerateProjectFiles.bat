@ECHO OFF

ECHO =============================
ECHO Start Generate Project Files
ECHO =============================

@REM CMake가 설치되었는지 확인합니다.
@ECHO OFF
where cmake > nul 2>&1
if %ERRORLEVEL% EQU 0 (
    ECHO CMake is installed on this system...
) else (
    ECHO CMake is not installed on this system...
    GOTO:EOF
)

@REM Visual Studio 버전입니다. 
@REM 엔진은 Visual Studio 2022만 지원합니다.
SET visualstudio="Visual Studio 17 2022"

@REM 프로젝트 이름입니다.
SET PROJECT_NAME=%1

@REM 프로젝트 이름의 유효성을 검사합니다.
if %PROJECT_NAME% == "" (
    ECHO Project name is empty...
    GOTO:EOF
) else (
    ECHO Project name is "%PROJECT_NAME%"...
)

@REM 프로젝트 생성 후 Visual Studio를 실행할 지 여부입니다.
SET RUN_OPTION=%2
if "%RUN_OPTION%" == "on" (
    SET RUN="on"
) else if "%RUN_OPTION%" == "On" (
    SET RUN="on"
) else if "%RUN_OPTION%" == "off" (
    SET RUN="off"
) else if "%RUN_OPTION%" == "Off" (
    SET RUN="off"
) else (
    SET RUN="off"
)
ECHO Switch %RUN% run Visual Studio Solution...

SET SOLUTION_PATH=%~dp0..\..\Solution

if not exist "%SOLUTION_PATH%" (
    mkdir "%SOLUTION_PATH%"
)

SET CURRENT_PATH=%~dp0

PUSHD "%CURRENT_PATH%"
PUSHD "%SOLUTION_PATH%"

cmake .. -G %visualstudio% -A "x64"

if %RUN% == "on" (
    start %PROJECT_NAME%.sln
)

POPD
POPD

ECHO =============================
ECHO Done Generate Project Files
ECHO =============================