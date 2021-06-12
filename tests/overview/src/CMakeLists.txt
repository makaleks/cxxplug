# Allows `dlopen` to search in caller directory
#set(CMAKE_BUILD_RPATH $ORIGIN)

add_executable(overview "main.cpp")
target_link_libraries(overview cxxplug)
target_link_libraries(overview ${plugins_builtin})

if (WIN32)
    # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category
    list(APPEND cxx_flags
        "/WX" "/W4"
    )
else()
    list(APPEND cxx_flags
        # It is impossible to include all GCC warnings
        # https://stackoverflow.com/a/11720263
        "-Werror" "-Wall" "-Wextra" "-pedantic"
        # Used as convenient use if #ifdef
        "-Wno-multichar"
    )
endif()
target_compile_options(overview PRIVATE ${cxx_flags})

add_test(
    NAME overview-test
    COMMAND overview
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:overview>"
)

#target_link_libraries(overview libload_static)
#set_target_properties(overview PROPERTIES BUILD_RPATH_USE_ORIGIN TRUE)

#add_test(NAME overview-test COMMAND overview WORKING_DIRECTORY $<TARGET_FILE_DIR:overview>)
