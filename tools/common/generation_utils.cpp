#include "generation_utils.hpp"
#include "predefined_symbols.hpp"

#include <filesystem>
#include <algorithm>

using namespace std;

std::string get_include_guard_str (const std::string &destination_filepath) {
    //unsigned num_of_underscores = 0;
    //enum PrevCase {
    //    NONE,
    //    UPPER,
    //    LOWER
    //} prev_case;
    string result = filesystem::path(destination_filepath).filename().string();
    const string_view to_replace = "-.";
    for (auto &c : result) {
        if (string_view::npos != to_replace.find(c)) {
            c = '_';
        }
    }

    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

void write_include_guard_start (
    std::ofstream &out, const std::string &include_guard_str
) {
    out << "#ifndef "   << include_guard_str
        << "\n#define " << include_guard_str << " 0\n";
}

void write_include_guard_end (
    std::ofstream &out, const std::string &include_guard_str
) {
    out << "\n#endif // "   << include_guard_str;
}

std::ofstream& write_indent (std::ofstream &out, unsigned indent) {
    char indent_str[] = "    ";
    while (indent > 4) {
        out << indent_str;
        indent -= 4;
    }
    indent_str[indent] = '\0';
    out << indent_str;
    return out;
}
