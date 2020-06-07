#ifndef HIDE_SYMBOLS_HPP_INCLUDED
#define HIDE_SYMBOLS_HPP_INCLUDED 0

#include <string_view>
#include <list>

void hide_symbols (
    const std::string_view            &command_objcopy,
    const std::string_view            &lib_path,
    const std::list<std::string_view> symbols
);

#endif //  HIDE_SYMBOLS_HPP_INCLUDED
