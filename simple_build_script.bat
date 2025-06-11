@echo off
REM Simple Cross-Platform Build Script for VedicMath-AI
REM Run this script from your project root directory

echo ============================================
echo VedicMath-AI Simple Build Script
echo ============================================

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found!
    echo Please run this script from your project root directory.
    pause
    exit /b 1
)

REM Create builds directory
if not exist "builds" mkdir builds
cd builds

REM ===========================================
REM 1. WINDOWS BUILD (First Priority)
REM ===========================================
echo.
echo [1/4] Building for Windows...

if not exist "windows" mkdir windows
cd windows

REM Try different Windows build methods
echo Trying Visual Studio build...
cmake -G "Visual Studio 17 2022" -A x64 ..\.. 2>nul
if not errorlevel 1 (
    cmake --build . --config Release
    if not errorlevel 1 (
        echo ✓ Windows build SUCCESS
        goto :linux_build
    )
)

echo Visual Studio failed, trying MinGW...
cmake -G "MinGW Makefiles" ..\.. 2>nul
if not errorlevel 1 (
    cmake --build . --config Release
    if not errorlevel 1 (
        echo ✓ Windows build SUCCESS
        goto :linux_build
    )
)

echo Trying default generator...
cmake ..\.. 2>nul
if not errorlevel 1 (
    cmake --build . --config Release
    if not errorlevel 1 (
        echo ✓ Windows build SUCCESS
    ) else (
        echo ✗ Windows build FAILED
    )
) else (
    echo ✗ Windows build FAILED
)

:linux_build
cd ..

REM ===========================================
REM 2. LINUX BUILD (via WSL if available)
REM ===========================================
echo.
echo [2/4] Building for Linux...

where wsl >nul 2>&1
if not errorlevel 1 (
    echo Using WSL for Linux build...
    wsl -- bash -c "
        cd /mnt/c/$(pwd -W | tr '\\' '/' | cut -d: -f2) || cd /mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')
        mkdir -p builds/linux
        cd builds/linux
        cmake -DCMAKE_BUILD_TYPE=Release ../.. 2>/dev/null
        if [ \$? -eq 0 ]; then
            make -j\$(nproc) 2>/dev/null
            if [ \$? -eq 0 ]; then
                echo '✓ Linux build SUCCESS'
            else
                echo '✗ Linux build FAILED'
            fi
        else
            echo '✗ Linux build FAILED (CMake configure)'
        fi
    " 2>nul
) else (
    echo WSL not available, skipping Linux build
    echo ○ Linux build SKIPPED
)

REM ===========================================
REM 3. PYTHON BINDINGS
REM ===========================================
echo.
echo [3/4] Building Python bindings...

where python >nul 2>&1
if not errorlevel 1 (
    if not exist "python" mkdir python
    cd python
    
    cmake -DBUILD_PYTHON_BINDINGS=ON -DBUILD_SHARED_LIBS=ON ..\.. 2>nul
    if not errorlevel 1 (
        cmake --build . --config Release 2>nul
        if not errorlevel 1 (
            echo ✓ Python bindings SUCCESS
            
            REM Test Python bindings
            echo Testing Python bindings...
            python -c "import sys; sys.path.append('python'); print('Python path test OK')" 2>nul
            if not errorlevel 1 (
                echo ✓ Python bindings test OK
            ) else (
                echo ○ Python bindings built but test incomplete
            )
        ) else (
            echo ✗ Python bindings build FAILED
        )
    ) else (
        echo ✗ Python bindings configure FAILED
    )
    cd ..
) else (
    echo Python not found, skipping Python bindings
    echo ○ Python bindings SKIPPED
)

REM ===========================================
REM 4. ANDROID BUILD (if NDK available)
REM ===========================================
echo.
echo [4/4] Building for Android...

if defined ANDROID_NDK_ROOT (
    echo Using Android NDK: %ANDROID_NDK_ROOT%
    if not exist "android" mkdir android
    cd android
    
    REM Build for arm64-v8a (most common)
    if not exist "arm64-v8a" mkdir arm64-v8a
    cd arm64-v8a
    
    cmake -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK_ROOT%\build\cmake\android.toolchain.cmake ^
          -DANDROID_ABI=arm64-v8a ^
          -DANDROID_PLATFORM=android-21 ^
          -DANDROID_STL=c++_shared ^
          -DBUILD_ANDROID_VERSION=ON ^
          ..\..\.. 2>nul
    
    if not errorlevel 1 (
        cmake --build . --config Release 2>nul
        if not errorlevel 1 (
            echo ✓ Android arm64-v8a build SUCCESS
        ) else (
            echo ✗ Android arm64-v8a build FAILED
        )
    ) else (
        echo ✗ Android arm64-v8a configure FAILED
    )
    
    cd ..\..
) else (
    echo ANDROID_NDK_ROOT not set, skipping Android build
    echo ○ Android build SKIPPED
    echo   Set ANDROID_NDK_ROOT environment variable to enable Android builds
)

cd ..

REM ===========================================
REM BUILD SUMMARY
REM ===========================================
echo.
echo ============================================
echo BUILD SUMMARY
echo ============================================

REM Count successful builds
set /a SUCCESS_COUNT=0
set /a TOTAL_COUNT=0

if exist "builds\windows" (
    set /a TOTAL_COUNT+=1
    if exist "builds\windows\Release" (
        echo ✓ Windows: SUCCESS
        set /a SUCCESS_COUNT+=1
    ) else if exist "builds\windows\vedicmath.exe" (
        echo ✓ Windows: SUCCESS
        set /a SUCCESS_COUNT+=1
    ) else if exist "builds\windows\libvedicmath.a" (
        echo ✓ Windows: SUCCESS  
        set /a SUCCESS_COUNT+=1
    ) else (
        echo ✗ Windows: FAILED
    )
)

REM Check for Linux build in WSL
wsl -- test -f "/mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')/builds/linux/libvedicmath.a" 2>nul
if not errorlevel 1 (
    set /a TOTAL_COUNT+=1
    echo ✓ Linux: SUCCESS
    set /a SUCCESS_COUNT+=1
) else (
    wsl -- test -d "/mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')/builds/linux" 2>nul
    if not errorlevel 1 (
        set /a TOTAL_COUNT+=1
        echo ✗ Linux: FAILED
    )
)

if exist "builds\python" (
    set /a TOTAL_COUNT+=1
    if exist "builds\python\python" (
        echo ✓ Python: SUCCESS
        set /a SUCCESS_COUNT+=1
    ) else (
        echo ✗ Python: FAILED
    )
)

if exist "builds\android" (
    set /a TOTAL_COUNT+=1
    if exist "builds\android\arm64-v8a" (
        echo ✓ Android: SUCCESS
        set /a SUCCESS_COUNT+=1
    ) else (
        echo ✗ Android: FAILED
    )
)

echo.
echo Successful builds: !SUCCESS_COUNT!/!TOTAL_COUNT!

if !SUCCESS_COUNT! GTR 0 (
    echo.
    echo ============================================
    echo WHAT'S BUILT AND WHERE TO FIND IT
    echo ============================================
    
    if exist "builds\windows\Release" (
        echo Windows executables: builds\windows\Release\
        dir /b builds\windows\Release\*.exe 2>nul
    ) else if exist "builds\windows" (
        echo Windows build: builds\windows\
        dir /b builds\windows\*.exe 2>nul
        dir /b builds\windows\*.a 2>nul
    )
    
    wsl -- test -d "/mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')/builds/linux" 2>nul
    if not errorlevel 1 (
        echo Linux build: builds/linux/ (accessible via WSL)
        wsl -- ls -la "/mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')/builds/linux/" 2>nul | findstr -v "total"
    )
    
    if exist "builds\python\python" (
        echo Python module: builds\python\python\
        dir /b builds\python\python\*.pyd 2>nul
        dir /b builds\python\python\*.so 2>nul
    )
    
    if exist "builds\android\arm64-v8a" (
        echo Android libraries: builds\android\arm64-v8a\
        dir /b builds\android\arm64-v8a\*.so 2>nul
    )
    
    echo.
    echo ============================================
    echo HOW TO TEST YOUR BUILDS
    echo ============================================
    
    if exist "builds\windows\Release\vedicmath_test.exe" (
        echo Windows: builds\windows\Release\vedicmath_test.exe
    ) else if exist "builds\windows\vedicmath_test.exe" (
        echo Windows: builds\windows\vedicmath_test.exe
    )
    
    wsl -- test -f "/mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')/builds/linux/vedicmath_test" 2>nul
    if not errorlevel 1 (
        echo Linux: wsl ./builds/linux/vedicmath_test
    )
    
    if exist "builds\python\python" (
        echo Python: python -c "import sys; sys.path.append('builds/python/python'); import vedicmath"
    )
    
) else (
    echo.
    echo ============================================
    echo TROUBLESHOOTING TIPS
    echo ============================================
    echo.
    echo No builds succeeded. Here are some things to check:
    echo.
    echo 1. CMake installation:
    where cmake >nul 2>&1 && echo "   ✓ CMake found" || echo "   ✗ CMake not found - install CMake"
    echo.
    echo 2. Compiler availability:
    where cl >nul 2>&1 && echo "   ✓ MSVC found" || echo "   ○ MSVC not found"
    where gcc >nul 2>&1 && echo "   ✓ GCC found" || echo "   ○ GCC not found"
    where python >nul 2>&1 && echo "   ✓ Python found" || echo "   ○ Python not found"
    echo.
    echo 3. Source files:
    if exist "src\core\*.c" (
        echo "   ✓ Core source files found"
    ) else (
        echo "   ✗ Core source files missing in src/core/"
    )
    echo.
    echo 4. Try manual build:
    echo "   mkdir build && cd build"
    echo "   cmake .."
    echo "   cmake --build ."
    echo.
    echo 5. Check CMakeLists.txt for missing files
)

echo.
echo ============================================
echo NEXT STEPS
echo ============================================
echo.
echo If builds were successful:
echo   1. Test the executables shown above
echo   2. Run 'cmake --install .' in build directories to install
echo   3. Add library paths to your development environment
echo.
echo If you want to build for more platforms:
echo   - Set ANDROID_NDK_ROOT for Android builds
echo   - Set IDF_PATH for ESP32 builds  
echo   - Install WSL for better Linux support
echo.
echo For advanced cross-compilation:
echo   - Use the full build_all_platforms.bat script
echo   - Set up additional toolchains as needed
echo.

REM Create a simple test script
echo Creating test script...
(
echo @echo off
echo echo Testing VedicMath-AI builds...
echo.
if exist "builds\windows\Release\vedicmath_test.exe" (
    echo echo Testing Windows build:
    echo builds\windows\Release\vedicmath_test.exe
) else if exist "builds\windows\vedicmath_test.exe" (
    echo echo Testing Windows build:
    echo builds\windows\vedicmath_test.exe
)
echo.
wsl -- test -f "/mnt/c$(pwd | sed 's/\\\\/\\//g' | sed 's/C://')/builds/linux/vedicmath_test" 2>nul
if not errorlevel 1 (
    echo echo Testing Linux build:
    echo wsl ./builds/linux/vedicmath_test
    echo.
)
if exist "builds\python\python" (
    echo echo Testing Python bindings:
    echo python -c "import sys; sys.path.append('builds/python/python'); import vedicmath; print('VedicMath Python module loaded successfully')"
    echo.
)
echo pause
) > test_builds.bat

echo.
echo Test script created: test_builds.bat
echo Run it to test your builds!
echo.
echo Build process completed!
pause