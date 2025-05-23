cmake_minimum_required(VERSION 3.10)
project(VedicMath VERSION 1.0.0 LANGUAGES C)

# Option to enable OpenMP
option(USE_OPENMP "Enable OpenMP support" OFF)

# Option to build shared library
option(BUILD_SHARED_LIBS "Build shared library" OFF)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform-specific settings
if(MSVC)
    # Windows-specific settings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
    
    # Add Windows-specific libraries
    set(PLATFORM_LIBS "")
elseif(APPLE)
    # macOS-specific settings
    add_compile_options(-Wall -Wextra -pedantic)
    set(PLATFORM_LIBS "")
else()
    # Linux and other Unix-like systems
    add_compile_options(-Wall -Wextra -pedantic)
    set(PLATFORM_LIBS "m") # Link with math library
endif()

# Add OpenMP if enabled
if(USE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_C_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        add_compile_definitions(_OPENMP)
    endif()
endif()

# Define source files for the library with proper paths
set(VEDICMATH_SOURCES
    src/common/vedicmath_utils.c
    src/common/vedicmath_dispatcher.c
    src/common/vedicmath_operators.c
    src/core/ekadhikena_purvena.c
    src/core/nikhilam_navatashcaramam.c
    src/core/urdhva_tiryagbhyam.c
    src/core/yaavadunam.c
    src/core/antyayordasake.c
    src/core/paravartya_yojayet.c
    src/core/sankalana_vyavakalanabhyam.c
    src/core/shunyam_saamyasamuccaye.c
    src/core/ekanyunena_purvena.c
    src/core/puranapuranabhyam.c
    src/subsutra/vestanam.c
    src/subsutra/anurupyena.c
    src/dynamic/vedicmath_types.c
    src/dynamic/vedicmath_dynamic.c
    src/optimized/vedicmath_optimized.c
    src/vedic_core.c
)

# Create the library
if(BUILD_SHARED_LIBS)
    add_compile_definitions(VEDICMATH_BUILD_SHARED)
    if(MSVC)
        add_compile_definitions(VEDICMATH_DLL_EXPORTS)
    endif()
    add_library(vedicmath SHARED ${VEDICMATH_SOURCES})
else()
    add_library(vedicmath STATIC ${VEDICMATH_SOURCES})
endif()

target_include_directories(vedicmath PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create the basic test program
add_executable(vedicmath_test tests/vedicmath_test.c)
target_link_libraries(vedicmath_test vedicmath ${PLATFORM_LIBS})

# Create the comprehensive test suite
add_executable(vedicmath_test_suite 
    tests/vedicmath_test_suite.c
    tests/vedicmath_test_main.c
)
target_link_libraries(vedicmath_test_suite vedicmath ${PLATFORM_LIBS})

# Create the dynamic test program
add_executable(vedicmath_dynamic_test tests/vedicmath_dynamic_test.c)
target_link_libraries(vedicmath_dynamic_test vedicmath ${PLATFORM_LIBS})

# Create the benchmark program with platform-specific modifications
add_executable(vedicmath_benchmark
    benchmarks/benchmark_main.c
    benchmarks/vedicmath_benchmark.c
)
target_link_libraries(vedicmath_benchmark vedicmath ${PLATFORM_LIBS})

# Installation rules
include(GNUInstallDirs)
install(TARGETS vedicmath
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES 
    include/vedicmath.h
    include/vedicmath_types.h
    include/vedicmath_dynamic.h
    include/vedicmath_optimized.h
    include/vedicmath_platform.h
    include/vedic_core.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Add option to build with optimization
option(OPTIMIZE_FOR_NATIVE "Build with architecture-specific optimizations" OFF)
if(OPTIMIZE_FOR_NATIVE)
    if(MSVC)
        # Visual Studio equivalent of -march=native
        add_compile_options(/arch:AVX2)
    else()
        # GCC/Clang
        add_compile_options(-march=native -O3)
    endif()
endif()

# Add platform test 
add_executable(platform_test tests/platform_test.c)
target_link_libraries(platform_test vedicmath ${PLATFORM_LIBS})

# Print configuration summary
message(STATUS "VedicMath configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Library type: ${BUILD_SHARED_LIBS}")
message(STATUS "  OpenMP support: ${USE_OPENMP}")
message(STATUS "  Native optimization: ${OPTIMIZE_FOR_NATIVE}")
if(USE_OPENMP AND OpenMP_C_FOUND)
    message(STATUS "  OpenMP C flags: ${OpenMP_C_FLAGS}")
endif()