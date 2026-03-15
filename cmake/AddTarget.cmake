# This module adds wrappers to the standard "add binary" / "add library" functions.

function(gbox_add_binary TARGET)
    add_binary(${TARGET} ${ARGN})
    _gbox_add_plugin_target(${TARGET})
endfunction()

function(gbox_add_executable TARGET)
    add_executable(${TARGET} ${ARGN})
    _gbox_add_plugin_target(${TARGET})
endfunction()

function(gbox_add_library TARGET)
    add_library(${TARGET} ${ARGN})
    _gbox_add_plugin_target(${TARGET})
endfunction()

function(_gbox_add_plugin_target TARGET)
    # 1) keep the logical target-level dependency (useful for linking, etc.)
    add_dependencies(${TARGET} ${GBOX_MACROS_NAME})

    # 3) create a per-target stamp file that will only be generated AFTER
    #    the plugin target is built. We use the plugin target as a DEPENDS item.
    #    The custom command just 'touch'es the stamp; it will run after the plugin.
    set(_stamp_dir "${CMAKE_BINARY_DIR}/stamps")
    set(_stamp "${_stamp_dir}/${TARGET}-${GBOX_MACROS_NAME}.stamp")

    # ensure the stamp directory exists at build time
    add_custom_command(
        OUTPUT "${_stamp}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${_stamp_dir}"
        COMMAND ${CMAKE_COMMAND} -E touch "${_stamp}"
        DEPENDS ${GBOX_MACROS_NAME}                # <-- depends on the plugin target
        COMMENT "Stamp after building ${GBOX_MACROS_NAME} for ${TARGET}"
        VERBATIM
    )

    # Mark it as a generated source and add it to the target's sources.
    # This forces CMake to schedule the custom command (and therefore plugin build)
    # before compilation of the target's sources.
    set_source_files_properties("${_stamp}" PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE "${_stamp}")

    # 4) finally add the plugin compile option using TARGET_FILE (evaluated at build time)
    target_compile_options(${TARGET} PRIVATE
        -fplugin=$<TARGET_FILE:${GBOX_MACROS_NAME}>
    )
endfunction()
