## file: `./CMakeLists.txt`
## brief: This file defines the root of the project.

cmake_minimum_required(VERSION 3.28.3)
project(gbox VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set(PROJECT_SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_INC_PATH "${PROJECT_SOURCE_DIR}/inc")
set(PROJECT_CONF_PATH "${PROJECT_SOURCE_DIR}/conf")
set(PROJECT_TEST_PATH "${PROJECT_SOURCE_DIR}/test")

include(FetchContent)
include(${PROJECT_CONF_PATH}/AddTests.cmake)
include(GoogleTest)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(${PROJECT_SRC_PATH})
add_subdirectory(${PROJECT_TEST_PATH})

enable_testing()
