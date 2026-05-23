@echo off
setlocal

if "%PICO_SDK_PATH%"=="" (
    echo Error: PICO_SDK_PATH is not set.
    exit /b 1
)

if not exist build mkdir build
cd build

cmake -G Ninja ..
if errorlevel 1 exit /b 1

ninja
if errorlevel 1 exit /b 1

echo.
echo Build OK: build\picochrono.uf2
endlocal
