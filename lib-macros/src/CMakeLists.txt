## file: `lib-macros/src/CMakeLists.txt`
## brief: This file links the required C source files and builds a static library.

file(GLOB_RECURSE GBOX_MACROS_CPP_SRC "${GBOX_MACROS_SRC_PATH}/*.cpp")
add_executable(${GBOX_MACROS_NAME} ${GBOX_MACROS_CPP_SRC})

target_include_directories(
  ${GBOX_MACROS_NAME}
  PUBLIC
  ${GBOX_MACROS_INC}
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(${GBOX_MACROS_NAME}
  PRIVATE
  clangAST
  clangTooling
  clangBasic
  clangFrontend
)
