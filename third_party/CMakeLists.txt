#set(MY_TARGET_OUTPUT_DIRECTORY_BACKUP "${MY_TARGET_OUTPUT_DIRECTORY}")
set(MY_TARGET_OUTPUT_DIRECTORY "${MY_TARGET_OUTPUT_DIRECTORY/third_party}")
include(ExternalProject)

#set(CMAKE_BINARY_DIR "${MY_TARGET_OUTPUT_DIRECTORY/libload}")
ExternalProject_Add(libload_dependency
    SOURCE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/libload"
    INSTALL_COMMAND ""
    PREFIX          "libload_dependency"
    #BINARY_DIR "${MY_TARGET_OUTPUT_DIRECTORY/libload}"
)
# https://stackoverflow.com/a/29324527
set(
    LIBLOAD_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/libload/include"
    PARENT_SCOPE
)
set(
    LIBLOAD_LIBRARY_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/libload_dependency/src/libload_dependency-build/libload/lib"
)
add_library(libload STATIC IMPORTED GLOBAL)
set_target_properties(
    libload PROPERTIES IMPORTED_LOCATION
    "${LIBLOAD_LIBRARY_DIR}/libload${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
target_link_libraries(libload INTERFACE ${CMAKE_DL_LIBS})
set(libload ${libload} PARENT_SCOPE)

#set(CMAKE_BINARY_DIR "${MY_TARGET_OUTPUT_DIRECTORY/ecstrings}")
ExternalProject_Add(ecstrings_dependency
    SOURCE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/ecstrings"
    PREFIX          "ecstrings_dependency"
    INSTALL_COMMAND ""
)
# https://stackoverflow.com/a/29324527
set(
    ECSTRINGS_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/ecstrings/include"
    PARENT_SCOPE
)
set(
    ECSTRINGS_LIBRARY_DIR
    "${CMAKE_CURRENT_BINARY_DIR}/ecstrings_dependency/src/ecstrings_dependency-build/ecstrings/lib"
)
add_library(ecstrings STATIC IMPORTED GLOBAL)
set_target_properties(
    ecstrings PROPERTIES IMPORTED_LOCATION
    "${ECSTRINGS_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}ecstrings${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
set(ecstrings ${ecstrings} PARENT_SCOPE)

#set(MY_TARGET_OUTPUT_DIRECTORY "${MY_TARGET_OUTPUT_DIRECTORY_BACKUP}")
#unset(MY_TARGET_OUTPUT_DIRECTORY_BACKUP)
