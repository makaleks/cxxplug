#ifndef GET_SYMBOLS_HPP_INCLUDED
#define GET_SYMBOLS_HPP_INCLUDED 0

#include <list>
#include <string>

#include "../common/parse_config.hpp"
#include "args.hpp"

std::list<std::string> get_all_symbols (
    const std::string_view command_nm,
    const std::string_view lib_path
);

void print_all_symbols (
    const std::list<std::string> &all_symbols
);

#endif // GET_SYMBOLS_HPP_INCLUDED
