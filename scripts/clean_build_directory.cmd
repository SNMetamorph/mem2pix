@echo off
cd ..
cd build
if %ERRORLEVEL% EQU 0 (
    rmdir /S /Q "bin"
    rmdir /S /Q "cache"
    echo Build directory cleaned up
) else (
    echo Cleaning failed
    pause
)