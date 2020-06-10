#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstring>

struct Args {
    const std::string_view source_lib_static_path;
    const std::string_view destination_lib_static_path;
    const std::string_view command_nm;
    const std::string_view command_objcopy;
    const bool             verbose;

    void print () {
        std::cout << "\
Args:\n\
    source_lib_static_path:      \""  << source_lib_static_path      << "\"\n\
    destination_lib_static_path: \""  << destination_lib_static_path << "\"\n\
    command_nm:                  \""  << command_nm                  << "\"\n\
    command_objcopy:             \""  << command_objcopy             << "\"\n\
";
    }

    Args (
        const char *source_lib_path,
        const char *destination_lib_path,
        const char *nm,
        const char *objcopy,
        bool new_verbose
    )
        : source_lib_static_path(source_lib_path),
          destination_lib_static_path(destination_lib_path),
          command_nm(nm), command_objcopy(objcopy),
          verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-hide-linked-symbols [<VERBOSE>]\
 <source-lib-static> [<destination-lib-static>\
 [<path-to-nm-command> <path-to-objcopy-command>]]\
 \n\
";
    }
    static bool is_argv_wrong (
        int argc, char **argv,
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

        if (arg - argv + 1 >= argc) {
            std::cerr << "Error: requires at least 1 argument.\n";
            return true;
        }

        if (!std::filesystem::exists(*arg)) {
            std::cerr << "Error: Source file \""
                      << *arg << "\" does not exists.\n";
            return true;
        }
        *source_lib_path = *arg;
        arg++;

        if (arg - argv == argc) {
            *destination_lib_path = *(arg - 1);
        }
        else {
            if (
                std::filesystem::exists(*arg)
                && !std::filesystem::is_regular_file(*arg)
            ) {
                std::cerr << "Error: Destination file \""
                          << *arg << "\" exists and not regular.\n";
                return true;
            }
            *destination_lib_path = *arg;
            arg++;

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
        }

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
