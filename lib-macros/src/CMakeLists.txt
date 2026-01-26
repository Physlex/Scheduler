## file: `lib-macros/src/CMakeLists.txt`
## brief: This file links the required C source files and builds a static library.

set(LLVM_LINK_COMPONENTS Support)

file(GLOB_RECURSE GBOX_MACROS_CPP_SRC "${GBOX_MACROS_SRC_PATH}/*.cpp")
add_llvm_library(${GBOX_MACROS_NAME} MODULE ${GBOX_MACROS_CPP_SRC} PLUGIN_TOOL clang)
target_include_directories(${GBOX_MACROS_NAME} PUBLIC ${GBOX_MACROS_INC})
target_link_libraries(${GBOX_MACROS_NAME}
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
)
