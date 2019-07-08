cmake_minimum_required(VERSION 3.14)

project(goto-franework)

#
# Basic Goto-Framework Paths
#
set(GOTO_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(GOTO_INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(GOTO_TEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/test/")
set(GOTO_DEPS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps/")

#
# Compiler Options.
#
set(CMAKE_CXX_STANDARD          17)     # Enable C++ 17 (This might does not with older version of cmake. so forcing cmake version to 3.14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Enable compiler to force support C++ 17
set(CMAKE_CXX_EXTENSIONS        ON)     # Enable C++ Extendions.

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/permissive-") # Disable Non-Standard MSVC Features and Syntaxs.
    add_compile_options("/W4")          # Enable Warning Level 4
endif()

#
# Directories
#
include_directories(${Goto_C_FRONTEND_SRC_PATH})
add_subdirectory(${Goto_C_FRONTEND_INC_PATH})

#
# Testing Options.
#
option(ENABLE_TESTS "Enables building tests using GTest" ON)
if(ENABLE_TESTS)
    include(cmake/AddTest.cmake)
    add_subdirectory(${Goto_C_FRONTEND_TEST_PATH})
endif()

#
# Platform Pre-definitions
#
if(WIN32)
    add_definition(-DGOTO_PLATFORM_WINDOWS)
endif()

if (UNIX)
    add_definition(-DGOTO_PLATFORM_UNIX)
endif()

#
# Pre-definitions
# 
option(ENABLE_ALLOCATOR_USE_LARGE_PAGE "Enable using large page when its possible" ON)
if(ENABLE_ALLOCATOR_USE_LARGE_PAGE)
    add_definition(-DUSE_LARGE_PAGE)
endif()