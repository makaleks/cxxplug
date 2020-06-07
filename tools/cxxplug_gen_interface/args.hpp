#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <filesystem>
#include <array>
#include <cstring>

struct Args {
    const std::string_view config_file_path;
    const std::string_view output_dir_or_file_name;
    const bool             verbose;

    void print () {
        std::cout << "\
Args:\n\
    config_file_path:        \"" << config_file_path << "\"\n\
    output_dir_or_file_name: \"" << output_dir_or_file_name  << "\"\n\
";
    }

    Args (char **argv, const char *new_output_dir_or_file_name, bool new_verbose)
        : config_file_path(argv[1]),
          output_dir_or_file_name(new_output_dir_or_file_name),
          verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-gen-interface [<VERBOSE>] <config-file> [<output-dir-or-file-name>]\n\
";
    }
    static bool is_argv_wrong (
        int argc, char **argv, const char **out_dir_name, bool *out_verbose
    ) {
        char **arg = argv + 1; // Skip 'program name'

        if (arg - argv + 1 < argc && 0 == std::strcmp("VERBOSE", *arg)) {
            *out_verbose = true;
            arg++;
        }

        if (arg - argv + 1 > argc) {
            std::cerr << "Error: requires at least 1 argument.\n";
            return true;
        }


        if (!std::filesystem::exists(*arg)) {
            std::cerr << "Error: file \"" << *arg << "\" does not exists.\n";
            return true;
        }
        arg++;

        if (arg - argv < argc) {
            std::array trailing_slash = {'/', '\\'};
            if (std::filesystem::is_directory(*arg)) {
                if (
                    std::find(
                        trailing_slash.begin(), trailing_slash.end(),
                        std::string_view(*arg).back()
                    ) == trailing_slash.end()
                ) {
                    std::cerr << "Error: please, set the trailing slash for directory \""
                              << *arg << "\"\n";
                    return true;
                }
            }
            else if (std::filesystem::exists(*arg)) {
                if (!std::filesystem::is_regular_file(*arg)) {
                    std::cerr << "Error: \"" << *arg
                              << "\" is not a file.\n";
                    return true;
                }
            }
            *out_dir_name = *arg;
            arg++;
        }

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
