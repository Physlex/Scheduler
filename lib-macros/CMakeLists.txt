# Set up LLVM and clang dependencies...

set(LLVM_LINK_COMPONENTS Support)

find_package(LLVM REQUIRED CONFIG)
include(AddLLVM)

find_package(Clang REQUIRED CONFIG)
include(${Clang_DIR}/AddClang.cmake)

add_definitions(${LLVM_DEFINITIONS})

message(${CLANG_INCLUDE_DIRS})
message(${LLVM_INCLUDE_DIRS})

# Library creation
add_library(${GBOX_MACROS_NAME} STATIC ${GBOX_MACROS_SRC})
target_include_directories(${GBOX_MACROS_NAME}
  PUBLIC
  ${GBOX_MACROS_INC_PATH}
  ${GBOX_CORE_INC_PATH}
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(${GBOX_MACROS_NAME}
  PUBLIC
  clangAST
  clangTooling
  clangBasic
  clangFrontend
)

# Binary creation
add_executable(${GBOX_CLANG_WRAPPER_NAME} ${GBOX_MACROS_SRC_PATH}/main.cpp)
target_link_libraries(${GBOX_CLANG_WRAPPER_NAME} PRIVATE ${GBOX_MACROS_NAME})
