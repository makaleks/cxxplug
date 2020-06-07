#ifndef GET_SYMBOLS_HPP_INCLUDED
#define GET_SYMBOLS_HPP_INCLUDED 0

#include <list>
#include <string>

#include "../common/parse_config.hpp"
#include "args.hpp"

std::list<std::string> get_all_symbols (Args &args);

bool is_unwanted (const std::string &symbol, const Parsed &config);

void print_all_symbols (
    const std::list<std::string> &all_symbols, const Parsed &config
);

void remove_unwanted_symbols (
    std::list<std::string> *all_symbols, const Parsed &config
);

#endif // GET_SYMBOLS_HPP_INCLUDED
