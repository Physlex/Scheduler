## file: `test/CMakeLists.txt`
## brief: This file implements the linking and setup for test binaries.

file(GLOB_RECURSE GBOX_PROC_TEST_SRC "${GBOX_PROC_TEST_PATH}/*.cpp")
set(GBOX_PROC_TEST_LIBRARIES clangAST clangTooling clangBasic clangFrontend "${GBOX_PROC_NAME}") 
add_tests(
    SOURCES   ${GBOX_PROC_TEST_SRC}
    INCLUDES  ${GBOX_PROC_INC_PATH} ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS}
    LIBRARIES ${GBOX_PROC_TEST_LIBRARIES}
)
