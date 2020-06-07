#ifndef PARSE_CONFIG_HPP_INCLUDED
#define PARSE_CONFIG_HPP_INCLUDED 0

#include "cxxplug.hpp"

#include <string>
#include <list>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <iostream>

class Parsed {
    static bool is_impossible_to_extract_symbol (
        const std::string &line,
        std::string_view *prefix_out_ptr,
        std::string_view *symbol_out_ptr,
        std::string_view *suffix_out_ptr
    ) {
        // void symbol ()    or    int symbol
        //            ^                     ^
        auto rlast_pos = std::make_reverse_iterator(
            std::find(line.begin(), line.end(), '(')
        );
        const auto rend = line.rend();
        if (rlast_pos != line.rbegin()) {
            rlast_pos = std::find_if(rlast_pos, rend, ::isgraph);
            if (rend == rlast_pos) {
                return true;
            }
        }

        // void symbol ()
        //     ^     ^
        auto rfirst_pos = rlast_pos;
        rfirst_pos = std::find_if(rfirst_pos, rend, ::isspace);
        if (rend == rfirst_pos) {
            return true;
        }

        // void symbol ()
        //      ^     ^
        auto first_pos = rfirst_pos.base();
        auto last_pos  = rlast_pos.base();

        if (prefix_out_ptr && symbol_out_ptr && suffix_out_ptr) {
            *prefix_out_ptr = std::string_view(line.begin(), first_pos);
            *symbol_out_ptr = std::string_view(first_pos, last_pos);
            *suffix_out_ptr = std::string_view(last_pos, line.end());
        }
        return false;
    }
public:
    std::string interface_name;
    Uuid        interface_uuid;

    std::list<std::string> lines_of_includes;
    std::list<std::string> lines_of_symbols;

    void print () {
        std::cout << "\
Parsed:\n\
    interface_name: " << interface_name << "\n\
    interface_uuid: " << interface_uuid << "\n\
    lines_of_includes: [\n\
";
        for (const auto &line : lines_of_includes) {
            std::cout << "\
        \"" << line << "\"\n\
";
        }
        std::cout << "\
    ]\n\
    lines_of_symbols: [\n\
";
        for (const auto &line : lines_of_symbols) {
            std::cout << "\
        \"" << line << "\"\n\
        field:  \"" << to_field(line) << "\"\n\
        symbol: \"" << to_symbol(line) << "\"\n\
";
        }
        std::cout << "\
    ]\n\
";
    }

    Parsed () = default;
    Parsed (
        std::string            new_interface_name,
        Uuid                   new_interface_uuid,
        std::list<std::string> new_lines_of_includes,
        std::list<std::string> new_lines_of_symbols
    ) :
        interface_name(new_interface_name),
        interface_uuid(new_interface_uuid),
        lines_of_includes(new_lines_of_includes),
        lines_of_symbols(new_lines_of_symbols)
    {}

    static std::string_view to_symbol (const std::string &line) {
        std::string_view prefix, symbol, suffix;
        is_impossible_to_extract_symbol(line, &prefix, &symbol, &suffix);
        return symbol;
    }
    static std::string to_field (const std::string &line) {
        std::string_view prefix, symbol, suffix;
        is_impossible_to_extract_symbol(line, &prefix, &symbol, &suffix);

        std::string result;
        result.reserve(
            prefix.length() + 2 + symbol.length() + 1 + suffix.length()
        );
        result += prefix;
        result += "(*";
        result += symbol;
        result += ")";
        result += suffix;

        return result;
    }
    static bool is_wrong_include_line (const std::string &line) {
        std::string_view extension = ".h";
        auto pos = line.rfind(extension);
        if (
            std::string::npos == pos
            ||
            line.end() != std::find_if(
                line.begin() + pos + extension.length(), line.end(), ::isgraph
            )
        ) {
            return true;
        }
        return false;
    }
    static bool is_wrong_symbol_line (const std::string &line) {
        if(is_impossible_to_extract_symbol(line, nullptr, nullptr, nullptr)) {
            return true;
        }
        return false;
    }

    static Parsed get_parsed_config (const std::string_view file_path);
};

#endif //  PARSE_CONFIG_HPP_INCLUDED
