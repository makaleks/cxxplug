# https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html

function (_cxxplug_get_third_party_root)
    if (
        DEFINED OVERRIDE_THIRD_PARTY_SOURCE_DIR
        AND DEFINED OVERRIDE_THIRD_PARTY_BINARY_DIR
    )
        set(THIRD_PARTY_ROOT "${OVERRIDE_THIRD_PARTY_SOURCE_DIR}" PARENT_SCOPE)
    else ()
        set(THIRD_PARTY_ROOT "${CXXPLUG_SOURCE_DIR}/third_party" PARENT_SCOPE)
    endif ()
endfunction ()

function (_cxxplug_get_project_layout_final_constants)
    set(arg_options     "")
    set(
        arg_values
        "CHECK_INCLUDE_DIRECTORY_RELATIVE_PATH"
        "CHECK_STATIC_LIBRARY_RELATIVE_PATH"
        "CHECK_SHARED_LIBRARY_RELATIVE_PATH"
    )
    set(arg_multivalues "")
    cmake_parse_arguments(
        CXXPLUG "${arg_options}" "${arg_values}" "${arg_multivalues}" ${ARGN}
    )

    set(
        include_directory_relative_path
        "include"
    )
    if (
        DEFINED CXXPLUG_CHECK_INCLUDE_DIRECTORY_RELATIVE_PATH
        AND NOT "${CXXPLUG_CHECK_INCLUDE_DIRECTORY_RELATIVE_PATH}"
            STREQUAL "${include_directory_relative_path}"
    )
        message(
            FATAL_ERROR "Include directory path settings are not concordant."
        )
    endif()
    set(
        _CXXPLUG_INCLUDE_DIRECTORY_RELATIVE_PATH
        ${include_directory_relative_path}
        PARENT_SCOPE
    )

    set(
        static_library_relative_path
        "cxxplug/lib/cxxplug${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
    if (
        DEFINED CXXPLUG_CHECK_STATIC_LIBRARY_RELATIVE_PATH
        AND NOT "${CXXPLUG_CHECK_STATIC_LIBRARY_RELATIVE_PATH}"
            STREQUAL "${static_library_relative_path}"
    )
        message(
            FATAL_ERROR "Static library path settings are not concordant."
            "\nGot ${CXXPLUG_CHECK_STATIC_LIBRARY_RELATIVE_PATH}"
            "\nNed ${static_library_relative_path}"
        )
    endif()
    set(
        _CXXPLUG_STATIC_LIBRARY_RELATIVE_PATH
        ${static_library_relative_path}
        PARENT_SCOPE
    )

    set(
        shared_library_relative_path
        "cxxplug/lib/cxxplug${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )
    if (
        DEFINED CXXPLUG_CHECK_SHARED_LIBRARY_RELATIVE_PATH
        AND NOT "${CXXPLUG_CHECK_SHARED_LIBRARY_RELATIVE_PATH}"
            STREQUAL "${shared_library_relative_path}"
    )
        message(
            FATAL_ERROR "Shared library path settings are not concordant."
        )
    endif()
    set(
        _CXXPLUG_SHARED_LIBRARY_RELATIVE_PATH
        ${shared_library_relative_path}
        PARENT_SCOPE
    )

    _cxxplug_get_third_party_root()
    include("${THIRD_PARTY_ROOT}/libload/scripts/libload_get_project_targets.cmake")
    _libload_get_project_layout_final_constants()
    set(
        _CXXPLUG_LIBRARY_TARGET_DEPENDENCIES
        ${_LIBLOAD_LIBRARY_TARGET_LINK_DEPENDENCIES}
        PARENT_SCOPE
    )
endfunction ()

function (cxxplug_get_project_targets)
    set(arg_options     "")
    set(
        arg_values
        "SOURCE_BASE_DIR" "SOURCE_DIR_NAME" "BINARY_BASE_DIR" "CUSTOM_PREFIX"
        "CXXPLUG_TOOLS_PATH_VAR"
        "LIBLOAD_SOURCE_BASE_DIR" "LIBLOAD_SOURCE_DIR_NAME"
        "ECSTRINGS_SOURCE_BASE_DIR" "ECSTRINGS_SOURCE_DIR_NAME"
    )
    set(arg_multivalues "")
    cmake_parse_arguments(
        CXXPLUG "${arg_options}" "${arg_values}" "${arg_multivalues}" ${ARGN}
    )

    if (NOT DEFINED CXXPLUG_SOURCE_BASE_DIR)
        message(FATAL_ERROR "CXXPLUG_SOURCE_BASE_DIR must be set!")
    elseif (NOT DEFINED CXXPLUG_CXXPLUG_TOOLS_PATH_VAR)
        message(FATAL_ERROR "CXXPLUG_TOOLS_PATH_VAR must be set!")
    elseif (NOT DEFINED CXXPLUG_SOURCE_DIR_NAME)
        set (CXXPLUG_SOURCE_DIR_NAME "cxxplug")
    endif ()

    _cxxplug_get_third_party_root()
    if (
        DEFINED OVERRIDE_THIRD_PARTY_SOURCE_DIR
        AND DEFINED OVERRIDE_THIRD_PARTY_BINARY_DIR
    )
        set(CXXPLUG_SOURCE_DIR "${OVERRIDE_THIRD_PARTY_SOURCE_DIR}/cxxplug")
        set(CXXPLUG_BINARY_PREFIX "${OVERRIDE_THIRD_PARTY_BINARY_DIR}/cxxplug_dependency")
        set(
            CMD_ARGS
"-DOVERRIDE_THIRD_PARTY_SOURCE_DIR=${OVERRIDE_THIRD_PARTY_SOURCE_DIR}"
"-DOVERRIDE_THIRD_PARTY_BINARY_DIR=${OVERRIDE_THIRD_PARTY_BINARY_DIR}"
        )
        message("cxxplug/scripts defined: ${OVERRIDE_THIRD_PARTY_SOURCE_DIR}, ${OVERRIDE_THIRD_PARTY_BINARY_DIR}")
    else ()
        set(CXXPLUG_BINARY_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/cxxplug_dependency")
        #set(CMD "${CMAKE_COMMAND}")
        message("cxxplug/scripts not defined")
    endif ()

    if (NOT DEFINED CXXPLUG_LIBLOAD_SOURCE_DIR_NAME)
        set(CXXPLUG_LIBLOAD_SOURCE_DIR_NAME "libload")
    endif ()
    if (NOT DEFINED CXXPLUG_ECSTRINGS_SOURCE_DIR_NAME)
        set(CXXPLUG_ECSTRINGS_SOURCE_DIR_NAME "ecstrings")
    endif ()

    if (NOT DEFINED CXXPLUG_LIBLOAD_SOURCE_BASE_DIR)
        set(CXXPLUG_LIBLOAD_SOURCE_BASE_DIR ${THIRD_PARTY_ROOT})
    endif ()
    if (NOT DEFINED CXXPLUG_ECSTRINGS_SOURCE_BASE_DIR)
        set(CXXPLUG_ECSTRINGS_SOURCE_BASE_DIR ${THIRD_PARTY_ROOT})
    endif ()

    ExternalProject_Add(cxxplug_dependency
        CMAKE_ARGS      ${CMD_ARGS}
        SOURCE_DIR      "${CXXPLUG_SOURCE_BASE_DIR}/${CXXPLUG_SOURCE_DIR_NAME}"
        INSTALL_COMMAND ""
        PREFIX          "${CXXPLUG_BINARY_PREFIX}"
    )
    ExternalProject_Get_Property(cxxplug_dependency SOURCE_DIR BINARY_DIR)

    _cxxplug_get_project_layout_final_constants()

    # Include dir

    set(
        "${CUSTOM_PREFIX}cxxplug_include_directory"
        "${SOURCE_DIR}/${_CXXPLUG_INCLUDE_DIRECTORY_RELATIVE_PATH}"
        PARENT_SCOPE
    )

    # Library

    set(cxxplug_target_name "${CUSTOM_PREFIX}cxxplug")
    #set(cxxplug_static_target_name "${CUSTOM_PREFIX}cxxplug_static")
    #set(cxxplug_shared_target_name "${CUSTOM_PREFIX}cxxplug_shared")

    add_library(${cxxplug_target_name} INTERFACE IMPORTED GLOBAL)
    #add_library(${cxxplug_static_target_name} STATIC IMPORTED GLOBAL)
    #add_library(${cxxplug_shared_target_name} STATIC IMPORTED GLOBAL)

    #set_target_properties(
    #    ${cxxplug_static_target_name} PROPERTIES IMPORTED_LOCATION
    #    "${BINARY_DIR}/${_CXXPLUG_STATIC_LIBRARY_RELATIVE_PATH}"
    #)
    #set_target_properties(
    #    ${cxxplug_shared_target_name} PROPERTIES IMPORTED_LOCATION
    #    "${BINARY_DIR}/${_CXXPLUG_SHARED_LIBRARY_RELATIVE_PATH}"
    #)

    set(
        ${cxxplug_target_name} ${cxxplug_target_name} PARENT_SCOPE
    )
    #set(
    #    ${cxxplug_static_target_name} ${cxxplug_static_target_name} PARENT_SCOPE
    #)
    #set(
    #    ${cxxplug_shared_target_name} ${cxxplug_static_target_name} PARENT_SCOPE
    #)

    # Dependencies

    include(
"${CXXPLUG_LIBLOAD_SOURCE_BASE_DIR}/${CXXPLUG_LIBLOAD_SOURCE_DIR_NAME}/\
scripts/libload_get_project_targets.cmake"
    )
    libload_get_project_targets(
        SOURCE_BASE_DIR "${CXXPLUG_LIBLOAD_SOURCE_BASE_DIR}"
        SOURCE_DIR_NAME "${CXXPLUG_LIBLOAD_SOURCE_DIR_NAME}"
    )
    set(
        libload_include_directory
        "${libload_include_directory}"
        PARENT_SCOPE
    )
    set(libload ${libload_static} PARENT_SCOPE)

    include(
"${CXXPLUG_ECSTRINGS_SOURCE_BASE_DIR}/${CXXPLUG_ECSTRINGS_SOURCE_DIR_NAME}/\
scripts/ecstrings_get_project_targets.cmake"
    )
    ecstrings_get_project_targets(
        SOURCE_BASE_DIR "${CXXPLUG_ECSTRINGS_SOURCE_BASE_DIR}"
        SOURCE_DIR_NAME "${CXXPLUG_ECSTRINGS_SOURCE_DIR_NAME}"
    )
    set(
        ecstrings_include_directory
        ${ecstrings_include_directory}
        PARENT_SCOPE
    )
    set(ecstrings ${ecstrings_static} PARENT_SCOPE)

    # Tools directory

    set(
        "${CXXPLUG_CXXPLUG_TOOLS_PATH_VAR}"
        "${BINARY_DIR}/cxxplug/tools"
        PARENT_SCOPE
    )

    # Dependencies
    message("Dep: ${CXXPLUG_LIBRARY_TARGET_DEPENDENCIES} (${CMAKE_DL_LIBS}?)")
endfunction ()
