set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools")

list(
    APPEND common_sources
    "common/parse_config.hpp"       "common/parse_config.cpp"
    "common/generation_utils.hpp"   "common/generation_utils.cpp"
    "common/predefined_symbols.hpp" "common/predefined_symbols.cpp"
)
add_library(common_objects OBJECT ${common_sources})

list(
    APPEND cxxplug_gen_interface_sources
    "cxxplug_gen_interface.cpp"
    "cxxplug_gen_interface/args.hpp"
    "cxxplug_gen_interface/gen_interface.cpp"
    "cxxplug_gen_interface/gen_interface.hpp"
    "cxxplug_gen_interface/get_filepath.cpp"
    "cxxplug_gen_interface/get_filepath.hpp"
)
add_executable(
    cxxplug-gen-interface
    ${cxxplug_gen_interface_sources}
    $<TARGET_OBJECTS:common_objects>
)

list(
    APPEND cxxplug_gen_proxy_sources
    "cxxplug_gen_proxy.cpp"
    "cxxplug_gen_proxy/args.hpp"
    "cxxplug_gen_proxy/gen_proxy.hpp"
    "cxxplug_gen_proxy/gen_proxy.cpp"
)
add_executable(
    cxxplug-gen-proxy
    ${cxxplug_gen_proxy_sources}
    $<TARGET_OBJECTS:common_objects>
)

list(
    APPEND cxxplug_hide_linked_symbols_sources
    "cxxplug_hide_linked_symbols.cpp"
    "cxxplug_hide_linked_symbols/args.hpp"
    "cxxplug_hide_linked_symbols/get_symbols.hpp"
    "cxxplug_hide_linked_symbols/get_symbols.cpp"
    "cxxplug_hide_linked_symbols/hide_symbols.hpp"
    "cxxplug_hide_linked_symbols/hide_symbols.cpp"
)
add_executable(
    cxxplug-hide-linked-symbols
    ${cxxplug_hide_linked_symbols_sources}
    $<TARGET_OBJECTS:common_objects>
)

list(
    APPEND cxxplug_gen_collected_proxies_sources
    "cxxplug_gen_collected_proxies.cpp"
    "cxxplug_gen_collected_proxies/args.hpp"
    "cxxplug_gen_collected_proxies/gen_collection.hpp"
    "cxxplug_gen_collected_proxies/gen_collection.cpp"
)
add_executable(
    cxxplug-gen-collected-proxies
    ${cxxplug_gen_collected_proxies_sources}
    $<TARGET_OBJECTS:common_objects>
)
