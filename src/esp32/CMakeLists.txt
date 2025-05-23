# CMakeLists.txt for ESP32-S3 VedicMath-AI
cmake_minimum_required(VERSION 3.16)

# ESP32 specific configuration
set(ESP32_SUPPORTED_TARGETS esp32s3)

# Include ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Project configuration
project(vedicmath_esp32)

# ESP32-S3 specific compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DESP32_PLATFORM")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVEDICMATH_ESP32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")  # Optimize for size

# Optional features
option(CONFIG_ENABLE_WIFI "Enable WiFi data transmission" OFF)
option(CONFIG_ENABLE_BLUETOOTH "Enable Bluetooth communication" OFF)

if(CONFIG_ENABLE_WIFI)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_ENABLE_WIFI")
endif()

if(CONFIG_ENABLE_BLUETOOTH)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_ENABLE_BLUETOOTH")
endif()

# Component configuration would go in main/CMakeLists.txt