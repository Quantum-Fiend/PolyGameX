@echo off
echo ========================================
echo PolyGameX Build Script (Windows)
echo ========================================
echo.

REM Check for CMake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake 3.15 or higher
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

echo Configuring project with CMake...
cmake .. -G "Visual Studio 16 2019" -A x64
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake configuration failed
    cd ..
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed
    cd ..
    exit /b 1
)

cd ..

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Binaries located in: build\bin\Release
echo.
echo To run the demo:
echo   cd build\bin\Release
echo   PolyGameXDemo.exe
echo.

pause
