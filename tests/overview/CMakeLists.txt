set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/overview")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/overview")

include_directories("include")

add_subdirectory("plugins")
get_directory_property(subdir_includes DIRECTORY "plugins" INCLUDE_DIRECTORIES)
include_directories(${subdir_includes})

add_subdirectory("src")
