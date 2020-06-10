#ifndef HIDE_SYMBOLS_HPP_INCLUDED
#define HIDE_SYMBOLS_HPP_INCLUDED 0

#include <string>
#include <list>

void hide_symbols (
    const std::string_view       &command_objcopy,
    const std::string_view       &source_lib_path,
    const std::string_view       &destination_lib_path,
    const std::list<std::string> symbols
);

#endif //  HIDE_SYMBOLS_HPP_INCLUDED
