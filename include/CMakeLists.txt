list(
    APPEND PROJECT_HEADERS
    cxxplug.hpp
    #cxxplug/
    cxxplug/plug_loaded.hpp
    cxxplug/uuid.hpp
    cxxplug/i18n_provided.hpp
    #cxxplug/load_provider.hpp
    cxxplug/some_load_providers/libload_provider.hpp
    #cxxplug/source_provider.hpp
    cxxplug/some_source_providers/filesystem_dynlib_provider.hpp
)

foreach(header_path ${PROJECT_HEADERS})
    get_filename_component(header_directory ${header_path} DIRECTORY)
    file(
        COPY ${header_path}
        DESTINATION "${MY_TARGET_OUTPUT_DIRECTORY}/include/${header_directory}"
    )
endforeach()

set(PROJECT_HEADERS PROJECT_HEADERS PARENT_SCOPE)
