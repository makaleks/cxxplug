#ifndef SYMBOLS_SERIALIZATION_HPP_INCLUDED
#define SYMBOLS_SERIALIZATION_HPP_INCLUDED 0

#include <list>
#include <span>
#include <string>
#include <string_view>
#include <algorithm>

template <class It>
std::string serialize_symbols(It symbols_begin, It symbols_end) {
    std::string result;

    while (symbols_begin != symbols_end) {
        result.append(*symbols_begin);
        result.push_back('\n');

        symbols_begin++;
    }
    return result;
}

std::list<std::string_view> deserialize_symbols (
    const std::string_view &serialized
) {
    std::list<std::string_view> result;

    auto it  = serialized.cbegin();
    auto end = serialized.cend();
    while (it != end) {
        auto delimiter_it = std::find(it, end, '\n');
        result.push_back(std::string_view(it, delimiter_it));
        it = delimiter_it;
        if (it != end) {
            it++;
        }
    }
    return result;
}

#endif // SYMBOLS_SERIALIZATION_HPP_INCLUDED 
