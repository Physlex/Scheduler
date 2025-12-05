
file(GLOB_RECURSE C_SRC "${PROJECT_SRC_PATH}/*.c")

include_directories(${PROJECT_INC_PATH})

add_executable(${PROJECT_NAME} ${C_SRC})
