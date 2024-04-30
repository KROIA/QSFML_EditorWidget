@echo off
set buildFolder=build
set generator="Visual Studio 17 2022"
set platform=x64

REM Speichere den aktuellen Pfad ab
set projectRootPath=%CD%


REM Enable farbige Konsolenausgaben
SETLOCAL EnableExtensions DisableDelayedExpansion
for /F %%a in ('echo prompt $E ^| cmd') do (
  set "ESC=%%a"
)
SETLOCAL EnableDelayedExpansion
set RED=!ESC![31m
set GREEN=!ESC![32m
set YELLOW=!ESC![33m
set WHITE=!ESC![37m
set END_COLOR=!ESC![0m

mkdir %buildFolder%
cd %buildFolder%


REM Funktionsaufruf build(Debug, projectRootPath)
CALL :build Debug, "%projectRootPath%/installation"
CALL :build Release, "%projectRootPath%/installation"

pause
EXIT

REM Funktionsdefinition
:build
REM Lese Parameter 1
set buildType=%~1
REM Lese Parameter 2
set installPrefix=%~2

REM Erstelle Buildpfad
mkdir %buildType%
REM Bewege in den Buildpfad
cd %buildType%
REM cmake Befehl f�rs konfigurieren
cmake -G %generator% -A %platform% -DQT_DEPLOY=ON -DRELATIVE_BUILD_FOLDER="%buildFolder%" -DCMAKE_BUILD_TYPE=%buildType% -DCMAKE_INSTALL_PREFIX="%installPrefix%" -DCMAKE_CXX_FLAGS="/EHsc /MP" -DCMAKE_C_FLAGS="/EHsc /MP" "%projectRootPath%"
REM cmake Befehl f�rs kompilieren
cmake --build . --config %buildType% --target install

if %errorlevel% neq 0 (
    CALL::ECHO_COLOR "Build failed!", %RED%
    pause
) else (
    CALL::ECHO_COLOR "Build successful!", %GREEN%
)
cd ..
EXIT /B 0

:ECHO_COLOR 
echo %2 %1 %END_COLOR%
EXIT /B 0