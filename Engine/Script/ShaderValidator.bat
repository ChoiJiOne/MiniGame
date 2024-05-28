@echo off

SET src=%1
SET validator=%~dp0..\Bin\glslangValidator.exe

for %%i in ("%src%") do (
    set "output=%%~nxi"
)

echo [CHECK] %src%

%validator% %src%