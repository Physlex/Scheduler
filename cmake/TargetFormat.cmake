file(GLOB_RECURSE SOURCE_FILES *.c *.cpp *.h *.hpp)
add_custom_target(format COMMAND clang-format -i ${SOURCE_FILES})
