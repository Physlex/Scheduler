# This module adds wrappers to the standard "add binary" / "add library" functions.

function(gbox_add_binary TARGET)
    add_binary(${TARGET} ${ARGN})
    _gbox_add_plugin_target(${TARGET})
endfunction()

function(gbox_add_library TARGET)
    add_library(${TARGET} ${ARGN})
    _gbox_add_plugin_target(${TARGET})
endfunction()

function(_gbox_add_plugin_target TARGET)
    set_property(
        TARGET ${TARGET}
        APPEND PROPERTY LINK_DEPENDS $<TARGET_FILE:${GBOX_MACROS_NAME}>
    )

    add_dependencies(${TARGET} ${GBOX_MACROS_NAME})
    target_compile_options(${TARGET} PRIVATE
        -fplugin=$<TARGET_FILE:${GBOX_MACROS_NAME}>
    )
endfunction()
