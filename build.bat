@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo   Student Record Management System - Build Script
echo ========================================================
echo.

where g++ >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [ERROR] g++ compiler was not found in your PATH!
    echo Please make sure MinGW or Dev-Cpp MinGW is installed.
    echo Default MinGW path: C:\MinGW\bin
    pause
    exit /b 1
)

echo [1/3] Compiling Windows visual style manifest resources...
windres resource.rc -O coff -o resource.res
if %ERRORLEVEL% neq 0 (
    echo [WARNING] windres failed or was not found. Continuing without manifest...
    set RES_PARAM=
) else (
    set RES_PARAM=resource.res
)

echo [2/3] Compiling pure C++ sources with g++ (std=c++11)...
g++ -std=c++11 -Wall -Wextra -O2 main.cpp MainWindow.cpp Student.cpp StudentManager.cpp %RES_PARAM% -o StudentRecordSystem.exe -mwindows -lcomctl32

if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Compilation failed! Check the error messages above.
    pause
    exit /b %ERRORLEVEL%
)

echo [3/3] Build succeeded! Generated: StudentRecordSystem.exe
echo.
echo ========================================================
echo   Application is ready to run!
echo ========================================================
echo.
set /p RUN_CHOICE="Do you want to launch the application now? (y/n): "
if /i "%RUN_CHOICE%"=="y" (
    start "" StudentRecordSystem.exe
)

endlocal

