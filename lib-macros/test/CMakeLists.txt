## file: `test/CMakeLists.txt`
## brief: This file implements the linking and setup for test binaries.

file(GLOB_RECURSE GBOX_MACROS_TEST_SRC "${GBOX_MACROS_TEST_PATH}/*.cpp")
add_tests(
    SOURCES ${GBOX_MACROS_TEST_SRC} INCLUDES ${GBOX_MACROS_INC_PATH} LIBRARIES ${GBOX_MACROS_NAME}
)
