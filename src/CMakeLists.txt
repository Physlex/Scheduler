## file: `src/CMakeLists.txt`
## brief: This file links the required C source files and builds a static library.

file(GLOB_RECURSE C_SRC "${PROJECT_SRC_PATH}/*.c")
add_library("${PROJECT_NAME}-core" STATIC ${C_SRC})
target_include_directories("${PROJECT_NAME}-core" PUBLIC ${PROJECT_INC_PATH})
