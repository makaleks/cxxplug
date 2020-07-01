add_subdirectory("plugin_human")

list(APPEND plugins_builtin plugin_human_builtin)
get_directory_property(
    subdir_includes DIRECTORY "plugin_human" INCLUDE_DIRECTORIES
)
include_directories(${subdir_includes})

set(plugins_builtin ${plugins_builtin} PARENT_SCOPE)

#file(GLOB SRC_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*/*.c")
#
#set(OUTDIR_BACKUP "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
#
#foreach( src ${SRC_LIST} )
#    # https://stackoverflow.com/a/30062299
#    string(REGEX REPLACE "\\.[^.]*$" "" output_path ${src})
#
#    get_filename_component(output_directory ${src} DIRECTORY)
#    get_filename_component(output_filename  ${src} NAME_WLE)
#    set(full_directory "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${output_directory}")
#    #if(NOT EXISTS "${full_directory}")
#    #    add_custom_target(
#    #        "my-buildtime-mkdir-${output_directory}"
#    #        ALL COMMAND
#    #        ${CMAKE_COMMAND} -E make_directory "${full_directory}"
#    #    )
#    #endif()
#
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTDIR_BACKUP}/${output_directory}")
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTDIR_BACKUP}/${output_directory}")
#
#    #add_library("${output_filename}" SHARED "${src}")
#    add_library("${output_filename}" STATIC "${src}")
#    set_target_properties("${output_filename}" PROPERTIES PREFIX "")
#    target_compile_definitions("${output_filename}" PRIVATE -DTEST_SHARED_BUILD)
#endforeach( src ${SRC_LIST} )

