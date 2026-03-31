set(GBOX_MACROS_SRC_PATH "${GBOX_MACROS_PATH}/src")
set(GBOX_MACROS_INC_PATH "${GBOX_MACROS_PATH}/inc")
set(GBOX_MACROS_TEST_PATH "${GBOX_MACROS_PATH}/test")

set(LLVM_LINK_COMPONENTS Support)

find_package(LLVM REQUIRED CONFIG)
include(AddLLVM)

find_package(Clang REQUIRED CONFIG)
include(${Clang_DIR}/AddClang.cmake)

add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(${GBOX_MACROS_SRC_PATH})
add_subdirectory(${GBOX_MACROS_TEST_PATH})

add_executable(${GBOX_CLANG_WRAPPER_NAME} "${GBOX_MACROS_SRC_PATH}/main.cpp")

target_include_directories(${GBOX_CLANG_WRAPPER_NAME}
  PUBLIC
  ${GBOX_MACROS_INC_PATH}
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(${GBOX_CLANG_WRAPPER_NAME}
  PRIVATE
  clangAST
  clangTooling
  clangBasic
  clangFrontend
  "${GBOX_MACROS_NAME}-core"
)
