@ECHO off

ECHO ======================
ECHO Start Project Package
ECHO ======================

SET SOLUTION_PATH=%~dp0..\..\Solution

if not exist %SOLUTION_PATH% (
    ECHO Can't find Visual Studio Solution...
    GOTO:EOF
)

SET MODE=%1

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