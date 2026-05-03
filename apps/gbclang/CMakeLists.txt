# Binary creation
add_executable(${GBCLANG_NAME} ${GBCLANG_SRC_PATH}/main.cpp)
target_link_libraries(${GBCLANG_NAME} PRIVATE ${GBOX_PROC_NAME})
target_include_directories(${GBCLANG_NAME} PUBLIC ${GBCLANG_INC_PATH})
