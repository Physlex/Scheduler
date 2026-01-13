# ADDEXAMPLESamples.cmake
#
# This file converts a set of globbed source files into example binaries.

function(add_examples)
    # Parse keyword arguments:
    #   SOURCES   - list of source files
    #   INCLUDES  - list of include dirs
    #   LIBRARIES - list of libraries
    cmake_parse_arguments(
        ADDEXAMPLES
        ""  # no options
        ""  # no single-value args
        "SOURCES;INCLUDES;LIBRARIES"  # multi-value args
        ${ARGN}
    )

    message("TEST ${ADDEXAMPLES_SOURCES}")
    message("TEST ${ADDEXAMPLES_INCLUDES}")
    message("TEST ${ADDEXAMPLES_LIBRARIES}")

    foreach(EX IN LISTS ADDEXAMPLES_SOURCES)
        cmake_path(GET EX STEM EX_NAME)
        set(RUNTIME_NAME "ex-${EX_NAME}")

        add_executable(${RUNTIME_NAME} ${EX})

        target_include_directories(
            ${RUNTIME_NAME} PRIVATE ${ADDEXAMPLES_INCLUDES}
        )

        target_link_libraries(
            ${RUNTIME_NAME} PRIVATE ${ADDEXAMPLES_LIBRARIES}
        )
    endforeach()
endfunction()
