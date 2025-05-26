cmake_minimum_required(VERSION 3.10)
project(VedicMathAI VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(USE_OPENMP "Enable OpenMP support" OFF)
option(OPTIMIZE_FOR_NATIVE "Build with architecture-specific optimizations" OFF)
option(ENABLE_DATASET_LOGGING "Enable comprehensive dataset logging" ON)
option(ENABLE_SYSTEM_MONITORING "Enable system resource monitoring" ON)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)
option(BUILD_ESP32_VERSION "Build for ESP32 platform" OFF)

# Platform detection
if(WIN32)
    add_definitions(-DVEDICMATH_PLATFORM_WINDOWS)
    set(PLATFORM_LIBS "")
elseif(APPLE)
    add_definitions(-DVEDICMATH_PLATFORM_MACOS)
    set(PLATFORM_LIBS "")
elseif(UNIX)
    add_definitions(-DVEDICMATH_PLATFORM_LINUX)
    set(PLATFORM_LIBS "m") # Link with math library
endif()

# Compiler-specific settings
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
    if(BUILD_SHARED_LIBS)
        add_definitions(-DVEDICMATH_BUILD_SHARED -DVEDICMATH_DLL_EXPORTS)
    endif()
else()
    add_compile_options(-Wall -Wextra -pedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3)
    endif()
endif()

# OpenMP support
if(USE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_C_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        add_definitions(-D_OPENMP)
    endif()
endif()

# Native optimization
if(OPTIMIZE_FOR_NATIVE)
    if(MSVC)
        add_compile_options(/arch:AVX2)
    else()
        add_compile_options(-march=native -mtune=native)
    endif()
endif()

# Feature flags
if(ENABLE_DATASET_LOGGING)
    add_definitions(-DENABLE_DATASET_LOGGING)
endif()

if(ENABLE_SYSTEM_MONITORING)
    add_definitions(-DENABLE_SYSTEM_MONITORING)
endif()

# Source files organization
set(VEDICMATH_CORE_SOURCES
    # Core sutras
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

    # Enhanced division sutras
    src/core/dhvajanka_division.c
    src/core/nikhilam_division.c
    src/core/enhanced_division_dispatcher.c
    
    # Sub-sutras
    src/subsutra/vestanam.c
    src/subsutra/anurupyena.c
    
    # Common utilities
    src/common/vedicmath_utils.c
    src/common/vedicmath_dispatcher.c
    src/common/vedicmath_operators.c
    
    # Dynamic type system
    src/dynamic/vedicmath_types.c
    src/dynamic/vedicmath_dynamic.c
    
    # Optimized implementation
    src/optimized/vedicmath_optimized.c
    
    # NEW: Unified core layer
    src/core/vedic_core.c
    
    # NEW: System-aware dispatcher
    src/dispatch/dispatch_mixed_mode.c
)

# Header files
set(VEDICMATH_HEADERS
    include/vedicmath.h
    include/vedicmath_types.h
    include/vedicmath_dynamic.h
    include/vedicmath_optimized.h
    include/vedicmath_platform.h
    
    # NEW: Core headers
    include/vedic_core.h
    include/dispatch_mixed_mode.h
)

# Create the main library
if(BUILD_SHARED_LIBS)
    add_library(vedicmath SHARED ${VEDICMATH_CORE_SOURCES})
    if(WIN32)
        target_compile_definitions(vedicmath PRIVATE VEDICMATH_DLL_EXPORTS)
    endif()
else()
    add_library(vedicmath STATIC ${VEDICMATH_CORE_SOURCES})
endif()

target_include_directories(vedicmath 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(vedicmath ${PLATFORM_LIBS})

# Set properties for the library 
add_executable(test_division_sutras
    tests/test_division_sutras.c
)
target_link_libraries(test_division_sutras vedicmath ${PLATFORM_LIBS})

# Test executables
add_executable(vedicmath_test tests/vedicmath_test.c)
target_link_libraries(vedicmath_test vedicmath ${PLATFORM_LIBS})

add_executable(vedicmath_test_suite
    tests/vedicmath_test_suite.c
    tests/vedicmath_test_main.c
)
target_link_libraries(vedicmath_test_suite vedicmath ${PLATFORM_LIBS})

add_executable(vedicmath_dynamic_test tests/vedicmath_dynamic_test.c)
target_link_libraries(vedicmath_dynamic_test vedicmath ${PLATFORM_LIBS})

# Benchmark executables
add_executable(vedicmath_benchmark
    benchmarks/benchmark_main.c
    benchmarks/vedicmath_benchmark.c
)
target_link_libraries(vedicmath_benchmark vedicmath ${PLATFORM_LIBS})

# NEW: Enhanced benchmark with novel features
add_executable(vedicmath_enhanced_benchmark
    benchmarks/benchmark_main.c
    benchmarks/vedicmath_benchmark.c
    src/benchmarks/novel_benchmarking.c  # From our earlier artifact
)
target_link_libraries(vedicmath_enhanced_benchmark vedicmath ${PLATFORM_LIBS})

# NEW: Unified core demo
add_executable(vedic_core_demo
    examples/vedic_core_demo.c
)
target_link_libraries(vedic_core_demo vedicmath ${PLATFORM_LIBS})

# NEW: Unified core demo
add_executable(debug_test
    tests/debug_test.c
)
target_link_libraries(debug_test vedicmath ${PLATFORM_LIBS})

# NEW: System-aware dispatcher demo
add_executable(dispatch_demo
    examples/dispatch_demo.c
)
target_link_libraries(dispatch_demo vedicmath ${PLATFORM_LIBS})

# NEW: Dataset generation utility
add_executable(dataset_generator
    tools/dataset_generator.c
)
target_link_libraries(dataset_generator vedicmath ${PLATFORM_LIBS})

# Platform test
add_executable(platform_test tests/platform_test.c)
target_link_libraries(platform_test vedicmath ${PLATFORM_LIBS})

# ESP32 specific build
if(BUILD_ESP32_VERSION)
    add_definitions(-DESP32_PLATFORM)
    
    # ESP32-specific sources
    set(ESP32_SOURCES
        src/esp32/esp32_vedic_main.c
        src/esp32/esp32_optimized.c
        # Reduced set of core sutras for ESP32
        src/core/ekadhikena_purvena.c
        src/core/nikhilam_navatashcaramam.c
        src/core/urdhva_tiryagbhyam.c
        src/core/antyayordasake.c
        src/common/vedicmath_utils.c
    )
    
    add_executable(vedicmath_esp32 ${ESP32_SOURCES})
    target_compile_definitions(vedicmath_esp32 PRIVATE ESP32_PLATFORM)
    target_include_directories(vedicmath_esp32 PRIVATE include)
endif()

# Python bindings (optional)
if(BUILD_PYTHON_BINDINGS)
    find_package(Python3 COMPONENTS Interpreter Development)
    if(Python3_FOUND)
        # Create shared library specifically for Python
        add_library(vedicmath_py SHARED ${VEDICMATH_CORE_SOURCES})
        target_link_libraries(vedicmath_py ${PLATFORM_LIBS})
        target_include_directories(vedicmath_py PUBLIC include)
        
        # Set properties for Python module
        set_target_properties(vedicmath_py PROPERTIES
            PREFIX ""
            SUFFIX ".so"
            OUTPUT_NAME "vedicmath"
        )
        
        if(WIN32)
            set_target_properties(vedicmath_py PROPERTIES SUFFIX ".pyd")
        endif()
    else()
        message(WARNING "Python3 not found, skipping Python bindings")
    endif()
endif()

# Installation configuration
include(GNUInstallDirs)

# Install library
install(TARGETS vedicmath
    EXPORT VedicMathTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(FILES ${VEDICMATH_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install executables
install(TARGETS 
    vedicmath_test
    test_division_sutras
    vedicmath_test_suite 
    vedicmath_dynamic_test 
    vedicmath_benchmark
    vedicmath_enhanced_benchmark
    vedic_core_demo
    dispatch_demo
    dataset_generator
    platform_test
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install Python bindings if built
if(BUILD_PYTHON_BINDINGS AND Python3_FOUND)
    install(TARGETS vedicmath_py
        DESTINATION ${Python3_SITELIB}
    )
    install(FILES python/vedicmath_py.py
        DESTINATION ${Python3_SITELIB}
    )
endif()

# Create export configuration
install(EXPORT VedicMathTargets
    FILE VedicMathTargets.cmake
    NAMESPACE VedicMath::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VedicMath
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    VedicMathConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    cmake/VedicMathConfig.cmake.in
    VedicMathConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VedicMath
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VedicMathConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/VedicMathConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VedicMath
)

# Documentation generation (if Doxygen found)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()

# CTest configuration for automated testing
enable_testing()

add_test(NAME BasicTests COMMAND vedicmath_test)
add_test(NAME ComprehensiveTests COMMAND vedicmath_test_suite 15)
add_test(NAME DynamicTests COMMAND vedicmath_dynamic_test)
add_test(NAME PlatformTests COMMAND platform_test)

# Performance benchmarks as tests (with timeout)
add_test(NAME BenchmarkTests COMMAND vedicmath_benchmark 10000)
set_tests_properties(BenchmarkTests PROPERTIES TIMEOUT 60)

add_test(NAME EnhancedBenchmarkTests COMMAND vedicmath_enhanced_benchmark 1000)
set_tests_properties(EnhancedBenchmarkTests PROPERTIES TIMEOUT 120)

# Add division sutras test
add_test(NAME DivisionSutrasTests COMMAND division_sutras_test)
set_tests_properties(DivisionSutrasTests PROPERTIES TIMEOUT 30)

# Custom targets for development
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS vedicmath_test vedicmath_test_suite vedicmath_dynamic_test platform_test
    COMMENT "Running all tests"
)

add_custom_target(run_benchmarks
    COMMAND vedicmath_benchmark 100000
    COMMAND vedicmath_enhanced_benchmark 10000
    DEPENDS vedicmath_benchmark vedicmath_enhanced_benchmark
    COMMENT "Running performance benchmarks"
)

add_custom_target(generate_dataset
    COMMAND dataset_generator --count 50000 --output vedic_dataset.csv
    DEPENDS dataset_generator
    COMMENT "Generating comprehensive dataset"
)

# Code coverage (if using GCC/Clang)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(ENABLE_COVERAGE "Enable code coverage" OFF)
    if(ENABLE_COVERAGE)
        add_compile_options(--coverage)
        add_link_options(--coverage)
        
        add_custom_target(coverage
            COMMAND lcov --capture --directory . --output-file coverage.info
            COMMAND genhtml coverage.info --output-directory coverage_html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report"
        )
    endif()
endif()

# Print configuration summary
message(STATUS "VedicMath-AI Build Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Library type: ${BUILD_SHARED_LIBS}")
message(STATUS "  OpenMP support: ${USE_OPENMP}")
message(STATUS "  Native optimization: ${OPTIMIZE_FOR_NATIVE}")
message(STATUS "  Dataset logging: ${ENABLE_DATASET_LOGGING}")
message(STATUS "  System monitoring: ${ENABLE_SYSTEM_MONITORING}")
message(STATUS "  Python bindings: ${BUILD_PYTHON_BINDINGS}")
message(STATUS "  ESP32 version: ${BUILD_ESP32_VERSION}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Compiler: ${CMAKE_C_COMPILER_ID}")

if(USE_OPENMP AND OpenMP_C_FOUND)
    message(STATUS "  OpenMP flags: ${OpenMP_C_FLAGS}")
endif()

# Create examples directory structure
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/dispatch)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/esp32)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/benchmarks)

# Create build info header
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vedicmath_build_info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/vedicmath_build_info.h
)

target_include_directories(vedicmath PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)