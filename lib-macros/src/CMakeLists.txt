file(GLOB_RECURSE GBOX_MACROS_CPP_SRC "${GBOX_MACROS_SRC_PATH}/*.cpp")
list(REMOVE_ITEM GBOX_MACROS_CPP_SRC "${GBOX_MACROS_SRC_PATH}/main.cpp")

add_library(${GBOX_MACROS_NAME}-core STATIC ${GBOX_MACROS_CPP_SRC})

target_include_directories(
  ${GBOX_MACROS_NAME}-core
  PUBLIC
  ${GBOX_MACROS_INC_PATH}
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(${GBOX_MACROS_NAME}-core
  PRIVATE
  clangAST
  clangTooling
  clangBasic
  clangFrontend
)
