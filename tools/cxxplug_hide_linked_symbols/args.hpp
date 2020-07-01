#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstring>

struct Args {
    const std::string_view symbols_lib_static_path;
    const std::string_view source_lib_static_path;
    const std::string_view destination_lib_static_path;
    const std::string_view command_nm;
    const std::string_view command_objcopy;
    const bool             verbose;

    void print () {
        std::cout << "\
Args:\n\
    symbols_lib_static_path:     \""  << symbols_lib_static_path     << "\"\n\
    source_lib_static_path:      \""  << source_lib_static_path      << "\"\n\
    destination_lib_static_path: \""  << destination_lib_static_path << "\"\n\
    command_nm:                  \""  << command_nm                  << "\"\n\
    command_objcopy:             \""  << command_objcopy             << "\"\n\
";
    }

    Args (
        const char *symbols_lib_path,
        const char *source_lib_path,
        const char *destination_lib_path,
        const char *nm,
        const char *objcopy,
        bool new_verbose
    )
        : symbols_lib_static_path(symbols_lib_path),
          source_lib_static_path(source_lib_path),
          destination_lib_static_path(destination_lib_path),
          command_nm(nm), command_objcopy(objcopy),
          verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-hide-linked-symbols [<VERBOSE>]\
 <symbols-lib-static> <source-lib-static> <destination-lib-static>\
 [<path-to-nm-command> <path-to-objcopy-command>]\
 \n\
";
    }
    static bool is_argv_wrong (
        int argc, char **argv,
        const char **symbols_lib_path,
        const char **source_lib_path,
        const char **destination_lib_path,
        const char **command_nm_ptr,
        const char **command_objcopy_ptr,
        bool       *out_verbose
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

        if (arg - argv + 3 >= argc) {
            std::cerr << "Error: requires at least 3 arguments.\n";
            return true;
        }

        unsigned i = 0;

        for (i = 0; i < 2; i++) {
            if (!std::filesystem::exists(arg[i])) {
                std::cerr << "Error: File \""
                          << arg[i] << "\" does not exists.\n";
                return true;
            }
        }
        i++; // no check for the last file
        *symbols_lib_path     = arg[0];
        *source_lib_path      = arg[1];
        *destination_lib_path = arg[2];
        arg += i;

        if (arg - argv != argc) {
            if (arg - argv + 2 > argc) {
                std::cerr << "Error: Paths to commands \"nm\"\
and \"objcopy\" must be set both or none.";
                return true;
            }
            *command_nm_ptr      = arg[0];
            *command_objcopy_ptr = arg[1];
            arg += 2;
        }

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
