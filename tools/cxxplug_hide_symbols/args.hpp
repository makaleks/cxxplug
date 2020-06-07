#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstring>
#include <list>

#include "../common/symbols_serialization.hpp"

struct Args {
    const std::string_view            lib_static_path;
    const std::string_view            command_objcopy;
    const bool                        verbose;
    const std::list<std::string_view> symbols;

    void print () {
        std::cout << "\
Args:\n\
    lib_static_path: \""  << lib_static_path  << "\"\n\
    command_objcopy: \""  << command_objcopy  << "\"\n\
    symbols:\n\
";
        for (auto &s : symbols) {
            std::cout << "      " << s << "\n";
        }
    }

    Args (
        char **argv,
        const char *objcopy,
        std::list<std::string_view> new_symbols,
        bool new_verbose
    )
        : lib_static_path(argv[1]),
          command_objcopy(objcopy), symbols(new_symbols), verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-hide-symbols [<VERBOSE>] <lib-static>\
 [<path-to-objcopy-command>] SYMBOLS [<symbols_str>]\n\
";
    }
    static bool is_argv_wrong (
        int argc, char **argv,
        const char **command_objcopy_ptr,
        std::list<std::string_view> *symbols_ptr,
        bool *out_verbose
    ) {
        if (0 == system(nullptr)) {
            std::cerr << "Error: command processor unavailable.\n";
            return true;
        }

        char **arg = argv + 1; // Skip 'program name'

        if (arg - argv + 1 <= argc &&  0 == std::strcmp("VERBOSE", *arg)) {
            *out_verbose = true;
            arg++;
        }

        if (arg - argv + 1 > argc) {
            std::cerr << "Error: requires at least 1 argument.\n";
            return true;
        }

        unsigned i = 0;
        bool error_found = false;
        if (!std::filesystem::exists(*arg)) {
            std::cerr << "Error: File \"" << *arg
                      << "\" does not exists.\n";
            return true;
        }
        arg++;

        if (arg + 1 - argv <= argc && 0 != std::strcmp("SYMBOLS", *arg)) {
            *command_objcopy_ptr = arg[0];
            arg += 1;
        }
        //else if (arg - argv < argc) {
        //    std::cerr << "Error: 'objcopy'  command must be passed.\n";
        //    return true;
        //}

        if (arg - argv + 1 > argc || 0 != std::strcmp("SYMBOLS", *arg)) {
            std::cerr << "Error: \"SYMBOLS\" separator must be passed.\n";
            return true;
        }
        arg++;
        if (arg - argv + 1 <= argc) {
            *symbols_ptr = deserialize_symbols(*arg);
        }
        arg++;

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
