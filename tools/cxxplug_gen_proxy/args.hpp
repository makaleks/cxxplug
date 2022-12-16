#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstring>
#include <algorithm>

struct Args {
    const std::string_view config_file_path;
    // Suggestion: use implementation directory name
    const std::string_view plugin_impl_unique_name;
    const std::string_view output_dir_name;
    const bool             verbose;

    void print () {
        std::cout << "\
Args:\n\
    config_file_path: \""        << config_file_path << "\"\n\
    plugin_impl_unique_name: \"" << plugin_impl_unique_name << "\"\n\
    output_dir_name:  \""        << output_dir_name  << "\"\n\
";
    }

    Args (
        char **argv,
        const char *new_output_dir_name,
        bool new_verbose
    )
        : config_file_path(argv[1]),
          plugin_impl_unique_name(argv[2]),
          output_dir_name(new_output_dir_name),
          verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-gen-proxy [<VERBOSE>] <config-file> <plugin-impl-unique-name> \n\
 [<output-dir-name>] [<interface-file-path]\n\
# Suggestion: use plugin implementation directory name\n\
";
    }
    static bool is_argv_wrong (
        int        argc,
        char       **argv,
        const char **out_dir_name,
        const char **out_interface_file_path,
        bool       *out_verbose
    ) {
        char **arg = argv + 1; // Skip 'program name'

        if (arg - argv + 1 < argc && 0 == std::strcmp("VERBOSE", *arg)) {
            *out_verbose = true;
            arg++;
        }

        if (arg - argv + 2 > argc) {
            std::cerr << "Error: requires at least 2 arguments.\n";
            return true;
        }

        if (!std::filesystem::exists(*arg)) {
            std::cerr << "Error: file \"" << *arg << "\" does not exists.\n";
            return true;
        }
        arg++;

        // Skip <plugin-impl-unique-name>
        arg++;

        if (arg - argv < argc && std::filesystem::is_directory(*arg)) {
            std::array trailing_slash = {'/', '\\'};
            if (
                std::find(
                    trailing_slash.begin(), trailing_slash.end(),
                    std::string_view(*arg).back()
                ) == trailing_slash.end()
            ) {
                std::cerr << "Error: please, set the trailing slash for path \""
                          << *arg << "\"\n";
                return true;
            }
            *out_dir_name = *arg;
            arg++;
        }

        if (arg - argv < argc) {
            if (!std::filesystem::is_regular_file(*arg)) {
                std::cerr << "Error: file \"" << *arg
                          << "\" does not exists.\n";
                return true;
            }
            *out_interface_file_path = *arg;
            arg++;
        }

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
