## file: `test/CMakeLists.txt`
## brief: This file implements the linking and setup for test binaries.

file(GLOB_RECURSE SCHED_TESTS "${PROJECT_TEST_PATH}/*.cpp")
add_tests(SOURCES ${SCHED_TESTS} INCLUDES ${PROJECT_INC_PATH} LIBRARIES ${PROJECT_NAME})
