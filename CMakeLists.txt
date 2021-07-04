cmake_minimum_required(VERSION 3.12)
project("cxxplug" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#message("Objcopy: " ${CMAKE_OBJCOPY})
#message("nm:      " ${CMAKE_NM})
#message("type: " ${CMAKE_BUILD_TYPE})

include("output_layout.cmake")

add_subdirectory("third_party")
include_directories("${ECSTRINGS_INCLUDE_DIR}")
include_directories("${LIBLOAD_INCLUDE_DIR}")

include_directories("include")


add_subdirectory("include")
add_subdirectory("src")
add_subdirectory("scripts")

add_subdirectory("tools")

include("scripts/cxxplug_gen.cmake")
enable_testing()
add_subdirectory("tests")
