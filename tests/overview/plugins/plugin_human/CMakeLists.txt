unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugin_human"
)

# Sources
list(
    APPEND vasiliy_pupkin_sources
    "vasiliy_pupkin/vasiliy_pupkin.c"
)
list(
    APPEND ivan_susanin_sources
    "ivan_susanin/ivan_susanin.c"
)
list(
    APPEND gregor_eisenhorn_sources
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
# So the output will be like "ivan_susanin_proxy.a"

foreach( plugin_name ${plugins_dynamic} )
    add_library("${plugin_name}" MODULE "${${plugin_name}_sources}")
    set_target_properties("${plugin_name}" PROPERTIES PREFIX "")
    target_compile_definitions("${plugin_name}" PRIVATE -DTEST_SHARED_BUILD)
endforeach( plugin_name ${plugins_dynamic} )

# https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#how-can-i-generate-a-source-file-during-the-build
set(interface_conf "cxxplug_interface.conf")
set(interface_file "plugin_human_interface.hpp")
add_custom_command(
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/${interface_file}"
    COMMAND "cxxplug-gen-interface"
        ARGS "${CMAKE_CURRENT_SOURCE_DIR}/${interface_conf}"
             "${CMAKE_CURRENT_BINARY_DIR}/${interface_file}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${interface_conf}"
)
add_custom_target(interface-file DEPENDS "${interface_file}")

#include_directories("${CMAKE_CURRENT_BINARY_DIR}")

foreach( plugin_name ${plugins_integrated} )
    add_library("${plugin_name}" STATIC "${${plugin_name}_sources}")
    set_target_properties("${plugin_name}" PROPERTIES PREFIX "")

    unset(proxy_sources)
    list(
        APPEND proxy_sources
        "${CMAKE_CURRENT_BINARY_DIR}/${plugin_name}_proxy.hpp"
        "${CMAKE_CURRENT_BINARY_DIR}/${plugin_name}_proxy.cpp"
    )

    add_custom_command(
        OUTPUT ${proxy_sources}
        COMMAND "cxxplug-gen-proxy"
            ARGS
                "${CMAKE_CURRENT_SOURCE_DIR}/${interface_conf}"
                "${plugin_name}"
                "${CMAKE_CURRENT_BINARY_DIR}/"
                "${CMAKE_CURRENT_BINARY_DIR}/${interface_file}"
        DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/${interface_file}"
            "${CMAKE_CURRENT_SOURCE_DIR}/${interface_conf}"
    )
    add_library("${plugin_name}_proxy_tmp" STATIC ${proxy_sources})
    set_target_properties("${plugin_name}_proxy_tmp" PROPERTIES PREFIX "")

    add_custom_command(
        OUTPUT "${plugin_name}_proxy${CMAKE_STATIC_LIBRARY_SUFFIX}"
        COMMAND "cxxplug-hide-linked-symbols"
            ARGS
                "$<TARGET_FILE:${plugin_name}_proxy_tmp>"
                "${plugin_name}_proxy${CMAKE_STATIC_LIBRARY_SUFFIX}"
                "${CMAKE_NM}"
                "${CMAKE_OBJCOPY}"
        DEPENDS
            "${plugin_name}_proxy_tmp"
    )
    add_custom_target(
        "${plugin_name}_proxy_target"
        ALL DEPENDS
        "${plugin_name}_proxy${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )

endforeach( plugin_name ${plugins_integrated} )

