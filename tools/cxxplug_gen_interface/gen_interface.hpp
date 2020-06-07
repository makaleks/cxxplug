#ifndef CXXPLUG_GEN_INTERFACE_HPP_INCLUDED
#define CXXPLUG_GEN_INTERFACE_HPP_INCLUDED 0

#include <string>

#include "../common/parse_config.hpp"

void gen_interface (
    const std::string &destination_filepath, const Parsed &config
);

#endif // CXXPLUG_GEN_INTERFACE_HPP_INCLUDED
