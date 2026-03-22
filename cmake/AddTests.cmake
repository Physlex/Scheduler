# AddTests.cmake
#
# This file converts a set of globbed source files into tests binaries.

function(add_tests)
    # Parse keyword arguments:
    #   SOURCES   - list of source files
    #   INCLUDES  - list of include dirs
    #   LIBRARIES - list of libraries
    cmake_parse_arguments(
        ADDTEST
        ""  # no options
        ""  # no single-value args
        "SOURCES;INCLUDES;LIBRARIES"  # multi-value args
        ${ARGN}
    )

    foreach(TEST IN LISTS ADDTEST_SOURCES)
        cmake_path(GET TEST STEM TEST_NAME)
        set(RUNTIME_NAME "test-${TEST_NAME}")

        add_executable(${RUNTIME_NAME} ${TEST})

        target_include_directories(
            ${RUNTIME_NAME} PRIVATE ${ADDTEST_INCLUDES}
        )

        target_link_libraries(
            ${RUNTIME_NAME} PRIVATE ${ADDTEST_LIBRARIES} GTest::gtest GTest::gtest_main
        )

        add_test(NAME ${RUNTIME_NAME} COMMAND ${RUNTIME_NAME})
    endforeach()
endfunction()
