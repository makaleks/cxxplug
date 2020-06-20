cmake_minimum_required(VERSION 3.12)

project("cxxplug" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#message("Objcopy: " ${CMAKE_OBJCOPY})
#message("nm:      " ${CMAKE_NM})
#message("type: " ${CMAKE_BUILD_TYPE})

include("output_layout.cmake")

include_directories("include")

add_subdirectory("third_party")
include_directories("${ECSTRINGS_INCLUDE_DIR}")


add_subdirectory("include")
#add_subdirectory("src")

add_subdirectory("tools")

#enable_testing()
add_subdirectory("tests")
