set(MY_TARGET_OUTPUT_DIRECTORY "${MY_TARGET_OUTPUT_DIRECTORY}/third_party")
include(ExternalProject)

if (DEFINED OVERRIDE_THIRD_PARTY_SOURCE_DIR)
    set(THIRD_PARTY_ROOT "${OVERRIDE_THIRD_PARTY_SOURCE_DIR}")
    message("cxxplug/third_party defined: ${OVERRIDE_THIRD_PARTY_SOURCE_DIR}, ${OVERRIDE_THIRD_PARTY_BINARY_DIR}")
else ()
    set(THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
    message("cxxplug/third_party not defined")
endif ()

# libload

include("${THIRD_PARTY_ROOT}/libload/scripts/libload_get_project_targets.cmake")
libload_get_project_targets(
    SOURCE_BASE_DIR "${THIRD_PARTY_ROOT}"
    SOURCE_DIR_NAME "libload"
)
set(
    LIBLOAD_INCLUDE_DIR
    "${libload_include_directory}"
    PARENT_SCOPE
)
set(libload ${libload_static} PARENT_SCOPE)

# ecstrings

include("${THIRD_PARTY_ROOT}/ecstrings/scripts/ecstrings_get_project_targets.cmake")
ecstrings_get_project_targets(
    SOURCE_BASE_DIR "${THIRD_PARTY_ROOT}"
    SOURCE_DIR_NAME "ecstrings"
)
set(
    ECSTRINGS_INCLUDE_DIR
    "${ecstrings_include_directory}"
    PARENT_SCOPE
)
set(ecstrings ${ecstrings_static} PARENT_SCOPE)

