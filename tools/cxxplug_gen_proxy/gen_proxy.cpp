#include "gen_proxy.hpp"
#include "../common/generation_utils.hpp"
#include "../common/predefined_symbols.hpp"

#include <fstream>
#include <iostream>

using namespace std;

static string gen_base_proxy_name (const Args &args) {
    string result = args.plugin_impl_unique_name.data();

    const string_view to_replace = " -";
    for (auto &c : result) {
        if (string_view::npos != to_replace.find(c)) {
            c = '_';
        }
    }

    return result;
}

static ProxyGenerationResult gen_filenames (
    const string &base_proxy_name, const Args &args
) {
    string output_base_name = args.plugin_impl_unique_name.data();

    output_base_name = args.output_dir_name.data() + output_base_name;
    return ProxyGenerationResult{
        .header = output_base_name + "_proxy.hpp",
        .source = output_base_name + "_proxy.cpp"
    };
}

static void write_interface_include (
    ofstream &out, string_view interface_file_path
) {
    out << "#include \"" << interface_file_path << "\"\n";
}

static void write_const_proxy_declaration_base (
    ofstream &out, const string &base_proxy_name, const Parsed &config
) {
    out << "const " << config.interface_name << " " << base_proxy_name;
}

static void write_const_proxy_declaration (
    ofstream &out, const string &base_proxy_name, const Parsed &config
) {
    out << "\nextern ";
    write_const_proxy_declaration_base(out, base_proxy_name, config);
    out << ";\n";
}

static void write_hpp_include (
    ofstream &out, const Parsed &config, const string &proxy_header
) {
    out << "#include \"" << string(proxy_header) + "\"\n\n";
    for (auto &s : config.lines_of_includes) {
        out << "#include \"" << s << "\"\n";
    }
}

static void write_const_proxy_definition (
    ofstream &out, const string &base_proxy_name, const Parsed &config
) {
    out << "\n";
    write_const_proxy_declaration_base(out, base_proxy_name, config);
    out << " {\n";

    unsigned indent = 4;

    for (
        auto it = config.lines_of_symbols.cbegin();
        it != config.lines_of_symbols.cend();
        it++
    ) {
        auto as_symbol = Parsed::to_symbol(*it);
        write_indent(out, indent)
                << "." << as_symbol << " = " << as_symbol << ",\n";
    }

    if (!config.lines_of_symbols.empty()) {
        out << "\n";
    }
    write_indent(out, indent)
            << "." << PredefinedSymbols[SYMBOL_GET_IMPLEMENTATION_ID]
            << " = " << PredefinedSymbols[SYMBOL_GET_IMPLEMENTATION_ID]
            << ",\n";
    write_indent(out, indent)
            << "." << PredefinedSymbols[SYMBOL_GET_IMPLEMENTATION_NAME]
            << " = " << PredefinedSymbols[SYMBOL_GET_IMPLEMENTATION_NAME]
            << "\n";

    out << "};\n";
}

ProxyGenerationResult
generateProxyFiles (
    const Args &args, const Parsed &config, std::string_view interface_file_path
) {
    string base_proxy_name = gen_base_proxy_name(args);

    ProxyGenerationResult filenames = gen_filenames(base_proxy_name, args);

    ofstream output_hpp(filenames.header);
    string include_guard_str = get_include_guard_str(filenames.header);
    write_include_guard_start(output_hpp, include_guard_str);
    write_interface_include(output_hpp, interface_file_path);
    write_const_proxy_declaration(output_hpp, base_proxy_name, config);
    write_include_guard_end(output_hpp, include_guard_str);


    ofstream output_cpp(filenames.source);
    write_hpp_include(output_cpp, config, filenames.header);
    write_const_proxy_definition(output_cpp, base_proxy_name, config);

    return filenames;
}
