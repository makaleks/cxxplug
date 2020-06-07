#ifndef CXXPLUG_GET_FILEPATH_HPP_INCLUDED
#define CXXPLUG_GET_FILEPATH_HPP_INCLUDED 0

#include <string>
#include <string_view>
#include <array>
#include <fstream>

#include "cxxplug/uuid.hpp"
#include "../common/parse_config.hpp"

std::string get_interface_filepath (
    const Parsed     &config,
    std::string_view _directory_or_file_path = "./"
);

#endif // CXXPLUG_GET_FILEPATH_HPP_INCLUDED
