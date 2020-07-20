unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugin_human"
)

# Sources
list(
    APPEND vasiliy_pupkin
    "vasiliy_pupkin/vasiliy_pupkin.c"
)
list(
    APPEND ivan_susanin
    "ivan_susanin/ivan_susanin.c"
)
list(
    APPEND gregor_eisenhorn
    "gregor_eisenhorn/gregor_eisenhorn.c"
)

# Plugin types
list(
    APPEND plugins_dynamic
    "vasiliy_pupkin"
)
list(
    APPEND plugins_integrated
    "ivan_susanin"
    "gregor_eisenhorn"
)

# Dynamically-loaded
foreach( plugin_name ${plugins_dynamic} )
    add_library("${plugin_name}" MODULE "${${plugin_name}}")
    # So the output will be like "ivan_susanin_proxy.a"
    set_target_properties("${plugin_name}" PROPERTIES PREFIX "")
    target_compile_definitions("${plugin_name}" PRIVATE -DTEST_SHARED_BUILD)
    target_link_libraries("${plugin_name}" ${ecstrings})
endforeach( plugin_name ${plugins_dynamic} )

# Statically-linked
cxxplug_gen_integrated(
    LISTS_INSIDE
    INTERFACE_CONFIG "cxxplug_interface.conf"
    INTERFACE_NAME   "plugin_human"
    IMPLEMENTATIONS  ${plugins_integrated}
)
# Library target object returned, must be linked with final executable
set(plugin_human_builtin ${CXXPLUG_INTERFACE_BUILTIN} PARENT_SCOPE)
# Include directory with generated builtin-collection and interface headers
# returned
include_directories("${CXXPLUG_GENERATED_INCLUDE_DIR}")

