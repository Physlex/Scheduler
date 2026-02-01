## file: `./CMakeLists.txt`
## brief: This file defines the root of the project.

cmake_minimum_required(VERSION 3.28.3)
project(gbox VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set(PROJECT_CONF_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(PROJECT_EXAMPLE_PATH "${PROJECT_SOURCE_DIR}/examples")

include(${PROJECT_CONF_PATH}/AddTests.cmake)
include(${PROJECT_CONF_PATH}/AddExamples.cmake)
include(${PROJECT_CONF_PATH}/AddCustomTargets.cmake)

if(NOT CMAKE_C_COMPILER MATCHES "clang" OR NOT CMAKE_CXX_COMPILER MATCHES clang\\+\\+)
    message(FATAL_ERROR "clang improperly installed. Did you use `nix develop`?")
endif()

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

set(GBOX_MACROS_PATH ${PROJECT_SOURCE_DIR}/lib-macros)
set(GBOX_RUNTIME_PATH ${PROJECT_SOURCE_DIR}/lib-runtime)

add_subdirectory(${GBOX_MACROS_PATH})
add_subdirectory(${GBOX_RUNTIME_PATH})
add_subdirectory(${PROJECT_EXAMPLE_PATH})
