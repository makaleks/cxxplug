#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstring>

struct Args {
    const std::string_view config_file_path;
    const std::string_view lib_static_path;
    const std::string_view command_nm;
    const bool             verbose;

    void print () {
        std::cout << "\
Args:\n\
    config_file_path: \"" << config_file_path << "\"\n\
    lib_static_path: \""  << lib_static_path  << "\"\n\
    command_nm:      \""  << command_nm       << "\"\n\
";
    }

    Args (char **argv, const char *nm, bool new_verbose)
        : config_file_path(argv[1]),
          lib_static_path(argv[2]),
          command_nm(nm), verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-print-unwanted-symbols [<VERBOSE>] <config-file> <lib-static>\
 [<path-to-nm-command>]\n\
";
    }
    static bool is_argv_wrong (
        int argc, char **argv,
        const char **command_nm_ptr,
        bool *out_verbose
    ) {
        if (0 == system(nullptr)) {
            std::cerr << "Error: command processor unavailable.\n";
            return true;
        }

        char **arg = argv + 1; // Skip 'program name'

        if (arg - argv + 1 < argc && 0 == std::strcmp("VERBOSE", *arg)) {
            *out_verbose = true;
            arg++;
        }

        if (arg - argv + 2 >= argc) {
            std::cerr << "Error: requires at least 2 arguments.\n";
            return true;
        }

        unsigned i = 0;
        bool error_found = false;
        for (i = 0; i < 2; i++) {
            if (!std::filesystem::exists(arg[i])) {
                std::cerr << "Error: File \"" << arg[i]
                          << "\" does not exists.\n";
                error_found = true;
            }
        }
        if (error_found) {
            return true;
        }
        arg += i;

        if (arg + 1 - argv == argc) {
            *command_nm_ptr = arg[0];
        }
        //else if (arg - argv < argc) {
        //    std::cerr << "Error: 'nm'  command must be passed.\n";
        //    return true;
        //}
        arg += 2;

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
