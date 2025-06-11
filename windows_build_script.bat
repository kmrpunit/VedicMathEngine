@echo off
REM build_python_windows.bat - Windows build script for VedicMath Python bindings

echo ========================================
echo VedicMath-AI Python Bindings Builder
echo ========================================

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Python is not installed or not in PATH
    echo Please install Python from https://www.python.org/downloads/
    echo Make sure to check "Add Python to PATH" during installation
    pause
    exit /b 1
)

echo Python found: 
python --version

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from https://cmake.org/download/
    echo Or use: winget install Kitware.CMake
    pause
    exit /b 1
)

echo CMake found:
cmake --version | findstr /r "cmake version"

REM Create build directory
if not exist build mkdir build
cd build

echo.
echo ========================================
echo Configuring CMake with Python bindings
echo ========================================

REM Configure with Python bindings enabled
cmake .. -DBUILD_PYTHON_BINDINGS=ON ^
         -DENABLE_DATASET_LOGGING=ON ^
         -DENABLE_SYSTEM_MONITORING=ON ^
         -DCMAKE_BUILD_TYPE=Release

if errorlevel 1 (
    echo ERROR: CMake configuration failed
    echo.
    echo Common solutions:
    echo 1. Make sure Visual Studio or MinGW is installed
    echo 2. Try running from Visual Studio Developer Command Prompt
    echo 3. Install Python development headers
    pause
    exit /b 1
)

echo.
echo ========================================
echo Building VedicMath library
echo ========================================

REM Build the project
cmake --build . --config Release

if errorlevel 1 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo Testing Python bindings
echo ========================================

REM Test the Python bindings
cmake --build . --target test_python --config Release

echo.
echo ========================================
echo Build completed successfully!
echo ========================================

echo.
echo Python module location: %CD%\python\vedicmath.pyd
echo Python wrapper location: %CD%\python\vedicmath_py.py
echo.
echo To test manually:
echo   1. set PYTHONPATH=%CD%\python;%%PYTHONPATH%%
echo   2. python -c "import vedicmath_py; print(vedicmath_py.demo_vedic_math())"
echo.
echo To install for system-wide use:
echo   cmake --build . --target install_python --config Release
echo.

pause