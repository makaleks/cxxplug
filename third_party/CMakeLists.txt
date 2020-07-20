set(MY_TARGET_OUTPUT_DIRECTORY "${MY_TARGET_OUTPUT_DIRECTORY}/third_party")
include(ExternalProject)

# libload

include("libload/scripts/libload_get_project_targets.cmake")
libload_get_project_targets(
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libload"
)
set(
    LIBLOAD_INCLUDE_DIR
    "${libload_include_directory}"
    PARENT_SCOPE
)
set(libload ${libload_static} PARENT_SCOPE)

# ecstrings

include("ecstrings/scripts/ecstrings_get_project_targets.cmake")
ecstrings_get_project_targets(
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ecstrings"
)
set(
    ECSTRINGS_INCLUDE_DIR
    "${ecstrings_include_directory}"
    PARENT_SCOPE
)
set(ecstrings ${ecstrings_static} PARENT_SCOPE)

