# https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html

function (cxxplug_gen_integrated)
    # Each entry of list names a list variable with source file names
    set(arg_options     "LISTS_INSIDE")
    set(arg_values      "INTERFACE_CONFIG" "INTERFACE_NAME" "TOOLS_PATH")
    set(arg_multivalues "IMPLEMENTATIONS")
    cmake_parse_arguments(
        CXXPLUG "${arg_options}" "${arg_values}" "${arg_multivalues}" ${ARGN}
    )

    if (NOT DEFINED CXXPLUG_INTERFACE_CONFIG)
        message(
            FATAL_ERROR "Argument \"CXXPLUG_INTERFACE_CONFIG\" was not passed."
        )
    elseif (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}")
        message(
            FATAL_ERROR
            "Interface file \"${CXXPLUG_INTERFACE_CONFIG}\" does not exist."
        )
    endif ()

    set(cxxplug_gen_interface         "cxxplug-gen-interface")
    set(cxxplug_gen_proxy             "cxxplug-gen-proxy")
    set(cxxplug_hide_linked_symbols   "cxxplug-hide-linked-symbols")
    set(cxxplug_gen_collected_proxies "cxxplug-gen-collected-proxies")
    if (DEFINED CXXPLUG_TOOLS_PATH)
        set(
            cxxplug_gen_interface
            "${CXXPLUG_TOOLS_PATH}/${cxxplug_gen_interface}"
        )
        set(
            cxxplug_gen_proxy
            "${CXXPLUG_TOOLS_PATH}/${cxxplug_gen_proxy}"
        )
        set(
            cxxplug_hide_linked_symbols
            "${CXXPLUG_TOOLS_PATH}/${cxxplug_hide_linked_symbols}"
        )
        set(
            cxxplug_gen_collected_proxies
            "${CXXPLUG_TOOLS_PATH}/${cxxplug_gen_collected_proxies}"
        )
    endif ()

    # Sources output directory generation
    set(base_include_dir      "${CMAKE_CURRENT_BINARY_DIR}/include")
    set(generated_include_dir "${base_include_dir}/cxxplug_gen")
    set(builtin_include_dir   "${generated_include_dir}/builtin")
    file(
        MAKE_DIRECTORY "${builtin_include_dir}"
    )
    include_directories("${generated_include_dir}")

    # Interface header
    if (DEFINED CXXPLUG_INTERFACE_NAME)
        set(interface_name "${CXXPLUG_INTERFACE_NAME}")
    else ()
        get_filename_component(
            interface_name ${CXXPLUG_INTERFACE_CONFIG} NAME_WE
        )
    endif ()
    set(
        interface_header
        "${generated_include_dir}/${interface_name}_interface.hpp"
    )
    add_custom_command(
        OUTPUT  "${interface_header}"
        COMMAND "${cxxplug_gen_interface}"
            ARGS
                "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}"
                "${interface_header}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}"
    )
    add_custom_target(
        "${interface_name}_header_target" DEPENDS "${interface_header}"
    )

    # Proxy generation for each passed implementation
    unset(proxy_builtin_name_and_header_list)
    unset(proxy_builtin_dependencies)
    unset(proxy_builtin_libraries)
    foreach (implementation ${CXXPLUG_IMPLEMENTATIONS})
        if (CXXPLUG_LISTS_INSIDE)
            set(implementation_sources   ${${implementation}})
            set(implementation_base_name ${implementation})
        else ()
            set(implementation_sources ${implementation})
            get_filename_component(
                implementation_base_name ${implementation} NAME_WE
            )
        endif ()

        # Implementation
        add_library(
            "${implementation_base_name}" STATIC ${implementation_sources}
        )
        set_target_properties(
            "${implementation_base_name}" PROPERTIES PREFIX ""
        )

        # Proxy
        set(
            proxy_hpp
            "${CMAKE_CURRENT_BINARY_DIR}/${implementation_base_name}_proxy.hpp"
        )
        set(
            proxy_cpp
            "${CMAKE_CURRENT_BINARY_DIR}/${implementation_base_name}_proxy.cpp"
        )
        set(
            proxy_sources
            "${proxy_hpp}"
            "${proxy_cpp}"
        )
        add_custom_command(
            OUTPUT ${proxy_sources}
            COMMAND "${cxxplug_gen_proxy}"
                ARGS
                    "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}"
                    "${implementation_base_name}"
                    "${CMAKE_CURRENT_BINARY_DIR}/"
                    "${interface_header}"
            DEPENDS
                "${interface_header}"
                "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}"
        )
        add_library(
            "${implementation_base_name}_proxy_impl" STATIC ${proxy_sources}
        )
        set_target_properties(
            "${implementation_base_name}_proxy_impl" PROPERTIES PREFIX ""
        )

        # Merged implementation&proxy (no symbols localized)
        string(
            CONCAT proxy_merged_output
                "${CMAKE_CURRENT_BINARY_DIR}/"
                "${implementation_base_name}_proxy_merged"
                "${CMAKE_STATIC_LIBRARY_SUFFIX}"
        )
        add_custom_command(
            OUTPUT
                "${proxy_merged_output}"
            COMMAND "${CMAKE_LINKER}"
                ARGS
                    "--whole-archive" "--relocatable"
                    "$<TARGET_FILE:${implementation_base_name}_proxy_impl>"
                    "$<TARGET_FILE:${implementation_base_name}>"
                    "-o"
                    "${proxy_merged_output}"
            DEPENDS
                "$<TARGET_FILE:${implementation_base_name}_proxy_impl>"
                "$<TARGET_FILE:${implementation_base_name}>"
        )
        add_custom_target(
            "${implementation_base_name}_proxy_merged_target"
            ALL DEPENDS
            "${proxy_merged_output}"
        )

        # Proxy final
        string(
            CONCAT proxy_final_output
                "${CMAKE_CURRENT_BINARY_DIR}/"
                "${implementation_base_name}_proxy"
                "${CMAKE_STATIC_LIBRARY_SUFFIX}"
        )
        add_custom_command(
            OUTPUT "${proxy_final_output}"
            COMMAND "${cxxplug_hide_linked_symbols}"
                ARGS
                    "$<TARGET_FILE:${implementation_base_name}>"
                    "${proxy_merged_output}"
                    "${proxy_final_output}"
                    "${CMAKE_NM}"
                    "${CMAKE_OBJCOPY}"
            DEPENDS
                "${proxy_merged_output}"
                "${implementation_base_name}"
        )
        add_custom_target(
            "${implementation_base_name}_proxy_target"
            ALL DEPENDS
            "${proxy_final_output}"
        )

        # Prepare more data to generate collection after this loop
        list(
            APPEND proxy_builtin_name_and_header_list
            "${implementation_base_name}" "${proxy_hpp}"
        )
        list(
            APPEND proxy_builtin_dependencies
            ${proxy_sources}
        )
        list(
            APPEND proxy_builtin_libraries
            "${proxy_final_output}"
        )
    endforeach ()

    set(
        builtin_collection_sources
        "${builtin_include_dir}/${interface_name}_builtin.hpp"
        "${builtin_include_dir}/${interface_name}_builtin.cpp"
    )
    add_custom_command(
        OUTPUT ${builtin_collection_sources}
        COMMAND "${cxxplug_gen_collected_proxies}"
            ARGS
                "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}"
                "${interface_name}"
                "${builtin_include_dir}/"
                "${interface_header}"
                "SOURCES"
                ${proxy_builtin_name_and_header_list}
        DEPENDS
            "${interface_header}"
            "${CMAKE_CURRENT_SOURCE_DIR}/${CXXPLUG_INTERFACE_CONFIG}"
            ${proxy_builtin_dependencies}
    )
    add_library(
        "${interface_name}_builtin"
        STATIC
        ${builtin_collection_sources}
    )
    target_link_libraries(
        "${interface_name}_builtin" ${proxy_builtin_libraries}
    )

    # Return values
    set(CXXPLUG_GENERATED_BUILTIN "${${interface_name}_builtin}" PARENT_SCOPE)
    set(CXXPLUG_GENERATED_INCLUDE_DIR "${base_include_dir}" PARENT_SCOPE)
endfunction ()
