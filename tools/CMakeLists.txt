set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools")

list(
    APPEND common_sources
    "common/parse_config.hpp"       "common/parse_config.cpp"
    "common/generation_utils.hpp"   "common/generation_utils.cpp"
    "common/predefined_symbols.hpp" "common/predefined_symbols.cpp"
    "common/symbols_serialization.hpp"
)

list(
    APPEND cxxplug_gen_interface_sources
    ${common_sources}
    "cxxplug_gen_interface.cpp"
    "cxxplug_gen_interface/args.hpp"
    "cxxplug_gen_interface/gen_interface.cpp"
    "cxxplug_gen_interface/gen_interface.hpp"
    "cxxplug_gen_interface/get_filepath.cpp"
    "cxxplug_gen_interface/get_filepath.hpp"
)
add_executable(cxxplug-gen-interface ${cxxplug_gen_interface_sources})

list(
    APPEND cxxplug_gen_proxy_sources
    ${common_sources}
    "cxxplug_gen_proxy.cpp"
    "cxxplug_gen_proxy/args.hpp"
    "cxxplug_gen_proxy/gen_proxy.hpp"
    "cxxplug_gen_proxy/gen_proxy.cpp"
)

add_executable(cxxplug-gen-proxy ${cxxplug_gen_proxy_sources})

list(
    APPEND cxxplug_print_unwanted_symbols_sources
    ${common_sources}
    "cxxplug_print_unwanted_symbols.cpp"
    "cxxplug_print_unwanted_symbols/args.hpp"
    "cxxplug_print_unwanted_symbols/get_symbols.hpp"
    "cxxplug_print_unwanted_symbols/get_symbols.cpp"
)

add_executable(
    cxxplug-print-unwanted-symbols ${cxxplug_print_unwanted_symbols_sources}
)

list(
    APPEND cxxplug_hide_symbols_sources
    ${common_sources}
    "cxxplug_hide_symbols.cpp"
    "cxxplug_hide_symbols/args.hpp"
    "cxxplug_hide_symbols/hide_symbols.hpp"
    "cxxplug_hide_symbols/hide_symbols.cpp"
)

add_executable(
    cxxplug-hide-symbols ${cxxplug_hide_symbols_sources}
)
