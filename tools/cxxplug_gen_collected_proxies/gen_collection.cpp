#include "gen_collection.hpp"
#include "../common/generation_utils.hpp"
#include "../common/predefined_symbols.hpp"

#include <fstream>
#include <iostream>

using namespace std;

static string gen_base_collection_name (const Args &args) {
    string result = args.proxy_collection_unique_name.data();

    const string_view to_replace = " -";
    for (auto &c : result) {
        if (string_view::npos != to_replace.find(c)) {
            c = '_';
        }
    }

    return result;
}

static CollectionGenerationResult gen_filenames (
    const string &base_collection_name, const Args &args
) {
    string output_base_name = args.proxy_collection_unique_name.data();

    output_base_name = args.output_dir_name.data() + output_base_name;
    return CollectionGenerationResult{
        .header = output_base_name + "_builtin.hpp",
        .source = output_base_name + "_builtin.cpp"
    };
}

static void write_interface_include (
    ofstream &out, string_view interface_file_path
) {
    out << "#include \"" << interface_file_path << "\"\n";
}

static void write_const_collection_declaration_base (
    ofstream     &out,
    const string &base_collection_name,
    const Args   &args,
    const Parsed &config
) {
    //out << "const " << config.interface_name << " "
    out << config.interface_name << " "
            << base_collection_name << " [" << args.proxies.size() << "]";
}

static void write_const_collection_declaration (
    ofstream     &out,
    const string &base_collection_name,
    const Args   &args,
    const Parsed &config
) {
    out << "\nextern ";
    write_const_collection_declaration_base(
        out, base_collection_name, args, config
    );
    out << ";\n";
}

static void write_hpp_includes (
    ofstream     &out,
    const Args   &args,
    const Parsed &config,
    const string &proxy_header
) {
    out << "#include \"" << string(proxy_header) + "\"\n\n";
    for (auto &s : config.lines_of_includes) {
        out << "#include \"" << s << "\"\n";
    }
    if (!args.proxies.empty()) {
        out << "\n";
        for (auto &info : args.proxies) {
            out << "#include \"" << info.header_path << "\"\n";
        }
    }
}

static void write_const_collection_definition (
    ofstream     &out,
    const string &base_proxy_name,
    const Args   &args,
    const Parsed &config
) {
    out << "\n";
    write_const_collection_declaration_base(out, base_proxy_name, args, config);
    out << " {\n";

    unsigned indent = 4;

    for (
        auto it = args.proxies.cbegin();
        it != args.proxies.cend();
        it++
    ) {
        write_indent(out, indent) << it->var_name;
        if (it != std::prev(args.proxies.cend())) {
            out << ",";
        }
        out << "\n";
    }

    out << "};\n";
}

CollectionGenerationResult
generateCollectionFiles (
    const Args &args, const Parsed &config, std::string_view interface_file_path
) {
    string base_collection_name = gen_base_collection_name(args);

    CollectionGenerationResult filenames = gen_filenames(
        base_collection_name, args
    );

    ofstream output_hpp(filenames.header);
    string include_guard_str = get_include_guard_str(filenames.header);
    write_include_guard_start(output_hpp, include_guard_str);
    write_interface_include(output_hpp, interface_file_path);
    write_const_collection_declaration(
        output_hpp, base_collection_name, args, config
    );
    write_include_guard_end(output_hpp, include_guard_str);


    ofstream output_cpp(filenames.source);
    write_hpp_includes(output_cpp, args, config, filenames.header);
    write_const_collection_definition(
        output_cpp, base_collection_name, args, config
    );

    return filenames;
}
