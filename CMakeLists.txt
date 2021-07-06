cmake_minimum_required(VERSION 3.12.4)  

# Arduino Library Template 
set(lib pas-co2)

project(${lib} CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)

# Coverage Flags
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage -DPAS_CO2_LOGGER_ENABLED=0 -Wswitch")
set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")

# Search for include Files
include_directories(
    src/config
    src/corelib
    src/pal
)

# Header & Source Files
set(Headers
    src/config
    src/corelib
    src/pal
    )

set(Sources
    src/corelib/pas-co2-i2c.cpp
    src/corelib/pas-co2-pulse.cpp
    src/corelib/pas-co2-pwm-sw.cpp
    src/corelib/pas-co2-reg.cpp
    src/corelib/pas-co2-serial.cpp
    src/corelib/pas-co2-uart.cpp
    src/corelib/pas-co2.cpp
    )

# Add Library
add_library(${lib} ${Sources} ${Headers})

# Enable Testing with CTest
enable_testing()

# Add GoogleTest 
add_subdirectory(googletest)
# Add Tests Folder
add_subdirectory(test/unit)