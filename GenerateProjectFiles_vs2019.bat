@echo off

SET visualstudio=vs2019
SET project=CoinDash3D
SET script=Engine\Script\GenerateProjectFiles.bat

%script% %visualstudio% %project% On