#include "parse_config.hpp"

#include <array>
#include <fstream>
#include <filesystem>

using namespace std;

static bool getline_not_comments_and_blank (
    ifstream &input, string &line, const char *required_err_msg
) {
    while (true) {
        if (input.eof()) {
            if (required_err_msg) {
                throw ifstream::failure(required_err_msg);
            }
            return false;
        }
        getline(input, line);
        auto first_non_space = find_if_not(
            line.cbegin(), line.cend(), ::isspace
        );
        if (first_non_space != line.cend() && *first_non_space != '#') {
            return true;
        }
    }
}

struct LineType {
    bool (*checker)(const string&);
    list<string> &lines;
    const char *err_msg;
    bool is_optional = false;
};

static bool next_or_is_wrong (
    auto *ptr_check_wrong, bool *is_any_found,
    const auto &arr, const string &line, bool is_second = false
) {
    if (!(*ptr_check_wrong)->checker(line)) {
        return false;
    }
    else if (
        false == (*ptr_check_wrong)->is_optional
        && (
            !*is_any_found
            || *ptr_check_wrong == prev(arr.end())
        )
    ) {
        auto dest = *ptr_check_wrong + 1;
        if (is_second && arr.begin() != *ptr_check_wrong) {
            --*ptr_check_wrong;
        }
        while (*ptr_check_wrong != dest) {
            cerr << (*ptr_check_wrong)->err_msg << " \""
                 << line << "\"\n";
            ++*ptr_check_wrong;
        }
        return true;
    }
    *is_any_found = false;
    ++*ptr_check_wrong;
    return next_or_is_wrong(
        ptr_check_wrong, is_any_found, arr, line, true
    );
}

Parsed Parsed::get_parsed_config (const std::string_view file_path) {
    string line;

    if (!filesystem::is_regular_file(file_path)) {
        throw ifstream::failure(
            string("File \"") + file_path.data() + "\" does not exist"
        );
    }
    ifstream input(file_path.data());

    getline_not_comments_and_blank(
        input, line, "first line (interface name) not reached"
    );
    string interface_name = line;

    getline_not_comments_and_blank(
        input, line, "second line (interface uuid) not reached"
    );
    Uuid interface_uuid = line.c_str();

    list<string> lines_of_native_names;
    list<string> lines_of_includes;
    list<string> lines_of_symbols;

    array checkers = {
        LineType{
            Parsed::is_wrong_native_name_line, lines_of_native_names,
            "Error: not a native name format at line",
            true // optional
        },
        LineType{
            Parsed::is_wrong_include_line, lines_of_includes,
            "Error: not a header format at line"
        },
        LineType{
            Parsed::is_wrong_symbol_line,  lines_of_symbols,
            "Error: can't extract symbol from"
        },
    };


    bool is_any_found = false;
    auto cur = checkers.begin();

    while (!input.eof()) {
        if(!getline_not_comments_and_blank(input, line, nullptr)) {
            break;
        }
        if (next_or_is_wrong(&cur, &is_any_found, checkers, line)) {
            throw ifstream::failure("string format incorrenct");
        }
        is_any_found = true;
        cur->lines.push_back(line);
    }

    return Parsed(
        move(interface_name), move(interface_uuid),

        move(lines_of_native_names),
        move(lines_of_includes),
        move(lines_of_symbols)
    );
}
