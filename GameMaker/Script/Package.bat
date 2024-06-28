@ECHO off

ECHO ======================
ECHO Start Project Package
ECHO ======================

SET SOLUTION_PATH=%~dp0..\..\Solution

@REM 프로젝트가 존재하지 않으면 프로젝트를 생성합니다.
if not exist %SOLUTION_PATH% (
    ECHO Can't find Visual Studio Solution...
    GOTO:EOF
)

@REM 패키징할 모드입니다.
SET MODE=%1

@REM 패키징할 모드의 유효성을 검사합니다.
if "%MODE%" == "Debug" (
    ECHO Package "%MODE%" mode...
) else if "%MODE%" == "Release" (
    ECHO Package "%MODE%" mode...
) else if "%MODE%" == "RelWithDebInfo" (
    ECHO Package "%MODE%" mode...
) else if "%MODE%" == "MinSizeRel" (
    ECHO Package "%MODE%" mode...
) else (
    ECHO "%MODE%" is illegal mode...
    GOTO:EOF
)

SET CURRENT_PATH=%~dp0

PUSHD "%CURRENT_PATH%"
PUSHD "%SOLUTION_PATH%"

cpack -C %MODE% -G NSIS

POPD
POPD

ECHO ======================
ECHO Done Project Package
ECHO ======================