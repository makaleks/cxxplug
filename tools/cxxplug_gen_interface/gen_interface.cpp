#include "gen_interface.hpp"
#include "../common/generation_utils.hpp"
#include "../common//predefined_symbols.hpp"

#include <cctype>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static const char *tuple_include_line           = "tuple";
static const char *uuid_include_line            = "cxxplug/uuid.hpp";
static const char *ecstrings_include_line       = "ecstrings.h";
static const char *tuple_map_function_name      = "get_tuple_map";

static void write_includes (
    ofstream &out, const Parsed &config
) {
    if (config.lines_of_includes.empty()) {
        return;
    }
    out << "\n";
    out << "#include <"  << tuple_include_line << ">\n";
    out << "#include \"" << uuid_include_line  << "\"\n";
    out << "#include \"" << ecstrings_include_line  << "\"\n";
    out << "\n";
    for (auto const &s : config.lines_of_includes) {
        out << "#include \"" << s << "\"\n";
    }
}

static void write_function_interface_ids (
    ofstream &out, const Parsed &config, unsigned indent
) {
    write_indent(out, indent)
            << "static Uuid "
            << PredefinedSymbols[SYMBOL_GET_INTERFACE_ID] << " () {\n";
    indent += 4;
    write_indent(out, indent)
            << "return Uuid(\""
            << config.interface_uuid.name.data() << "\");\n";
    indent -= 4;
    write_indent(out, indent) << "}\n";

    write_indent(out, indent)
            << "static const char* "
            << PredefinedSymbols[SYMBOL_GET_INTERFACE_NAME]
            << " (ECStringType *lang) {\n";
    indent += 4;
    if (config.native_names.empty()) {
        write_indent(out, indent)
                << "return \"" << config.interface_name << "\";\n";
    }
    else {
        write_indent(out, indent)
                << "static const ECStringEntry names["
                << config.native_names.size() << "] = {\n";
        indent += 4;
        for (
            auto it = config.native_names.begin();
            it != config.native_names.end();
            it++
        ) {
            write_indent(out, indent)
                    << "{{.chars=\"" << it->lang_iso369_3.chars
                    << "\"}, \"" << it->str << "\"}";
            if (it != prev(config.native_names.end())) {
                out << ',';
            }
            out << '\n';
        }
        indent -= 4;
        write_indent(out, indent)
                << "};\n";
        write_indent(out, indent)
                << "return ecStringGetEntry(names, "
                << config.native_names.size() << ", lang);\n";
    }
    indent -= 4;
    write_indent(out, indent) << "}\n";
}

static void write_tuple_map (
    ofstream &out, const Parsed &config, unsigned indent
) {
    write_indent(out, indent)
            << "constexpr auto " << tuple_map_function_name << " () {\n";

    indent += 4;
    write_indent(out, indent) << "using std::make_tuple;\n\n";
    write_indent(out, indent) << "using std::tie;\n\n";
    write_indent(out, indent) << "return make_tuple(\n";

    indent += 4;
    auto itlast = config.lines_of_symbols.cend();
    itlast--;
    for (
        auto it = config.lines_of_symbols.cbegin();
        it != config.lines_of_symbols.cend();
        it++
    ) {
        auto as_symbol = Parsed::to_symbol(*it);
        write_indent(out, indent)
                << "tie(\"" << as_symbol << "\", " << as_symbol << ")";
        if (it != itlast) {
            out << ',';
        }
        out << '\n';
    }

    indent -= 4;
    write_indent(out, indent) << ");\n";

    indent -= 4;
    write_indent(out, indent) << "}\n";
}

static void write_interface (
    ofstream &out, const Parsed &config
) {
    out << "\n";

    unsigned indent = 0;
    out << "struct " << config.interface_name << " {\n";

    indent = 4;
    for (auto const &symbol : config.lines_of_symbols) {
        write_indent(out, indent) << Parsed::to_field(symbol) << ";\n";
    }

    out << "\n";
    write_indent(out, indent)
            << "// Make sure your plugin id is unique when updated\n";

    write_indent(out, indent) << "const void* (*"
            << PredefinedSymbols[SYMBOL_GET_IMPLEMENTATION_ID] << ")();\n";
    write_indent(out, indent) << "const char* (*"
            << PredefinedSymbols[SYMBOL_GET_IMPLEMENTATION_NAME]
            << ")(const char lang_iso369_3[4]);\n";

    write_function_interface_ids(out, config, indent);

    write_tuple_map(out, config, indent);

    out << "};\n";
}

void gen_interface (
    const std::string &destination_filepath, const Parsed &config
) {
    ofstream output_hpp(destination_filepath);
    string include_guard_str = get_include_guard_str(destination_filepath);

    write_include_guard_start(output_hpp, include_guard_str);

    write_includes(output_hpp, config);
    write_interface(output_hpp, config);

    write_include_guard_end(output_hpp, include_guard_str);
}
