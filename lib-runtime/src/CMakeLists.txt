## file: `src/CMakeLists.txt`
## brief: This file links the required C source files and builds a static library.

file(GLOB_RECURSE GBOX_RUNTIME_C_SRC "${GBOX_RUNTIME_SRC_PATH}/*.c")
add_library("${GBOX_RUNTIME_NAME}" STATIC ${GBOX_RUNTIME_C_SRC})
target_include_directories("${GBOX_RUNTIME_NAME}" PUBLIC ${GBOX_RUNTIME_INC_PATH})
