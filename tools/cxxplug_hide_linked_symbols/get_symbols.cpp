#include "get_symbols.hpp"
#include "../common/predefined_symbols.hpp"

#include <stdio.h>
#include <stdexcept>
#include <span>
#include <vector>
#include <iterator>
#include <iostream>

using namespace std;

static void skip_single_line (FILE *read_stream) {
    int character = 0;
    while (!feof(read_stream) && '\n' != character) {
        character = fgetc(read_stream);
    }
}

static int extract_next_symbol_from_output (
    list<string> &to_push, span<char> buffer, FILE *nm_output_stream
) {
    auto it = buffer.begin();
    auto end = buffer.end();

    int character = 0;

    bool finished = false;

    for (; it != end && !feof(nm_output_stream); it++) {
        character = fgetc(nm_output_stream);
        if (character != EOF) {
            if (' ' == character) {
                finished = true;
                break;
            }
            *it = static_cast<char>(character);
        }
    }
    if (feof(nm_output_stream)) {
        return 1;
    }
    if (it == end && !finished && !feof(nm_output_stream)) {
        throw overflow_error("buffer for symbols from \"nm\" too small");
    }
    while ('\n' != character && !feof(nm_output_stream)) {
        character = fgetc(nm_output_stream);
    }

    to_push.push_back(string(buffer.begin(), it));

    return 0;
}

std::list<std::string> get_all_symbols (
    const std::string_view command_nm,
    const std::string_view lib_path
) {
    // no sort, format with <symbol-name> before Space, exported symbols only
    const char *nm_args = " --no-sort --portability --extern-only ";
    string command = string(command_nm) + nm_args
                     + lib_path.data();

    FILE *nm_output_stream = nullptr;

    nm_output_stream = popen(command.c_str(), "r");
    if (!nm_output_stream) {
        throw domain_error("could not execute: \"" + command + "\"");
    }

    char symbol_name_buffer[64] = {0};

    span buffer_slice = symbol_name_buffer;

    int is_error = 0;
    list<string> result;

    skip_single_line(nm_output_stream);
    while (!is_error) {
        is_error = extract_next_symbol_from_output(
            result, buffer_slice, nm_output_stream
        );
    }

    pclose(nm_output_stream);
    nm_output_stream = nullptr;

    return result;
}

void print_all_symbols (
    const std::list<std::string> &all_symbols
) {
    cout << "Symbols:\n";
    for (auto &s : all_symbols) {
        cout << "    "  << s << "\n";
    }
}
