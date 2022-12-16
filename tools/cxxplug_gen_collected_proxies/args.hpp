#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED 0

#include <string_view>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <cstring>
#include <vector>
#include <algorithm>

struct ProxyInfo {
    std::string_view var_name;
    std::string_view header_path;
};

struct Args {
    const std::string_view       config_file_path;
    // Suggestion: use collection directory name
    const std::string_view       proxy_collection_unique_name;
    const std::string_view       output_dir_name;
    const std::vector<ProxyInfo> proxies;
    const bool                   verbose;

    void print () {
        std::cout << "\
Args:\n\
    config_file_path: \""        << config_file_path << "\"\n\
    proxy_collection_unique_name: \"" << proxy_collection_unique_name << "\"\n\
    output_dir_name:  \""        << output_dir_name  << "\"\n\
    proxies: {\n\
";
        for (auto it = proxies.cbegin(); it != proxies.cend(); it++) {
            std::cout << "\
        \"" << it->var_name << "\": \"" << it->header_path << "\"";
            if (it != std::prev(proxies.cend())) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
    }

    Args (
        char                   **argv,
        int                    argc,
        const char             *new_output_dir_name,
        std::vector<ProxyInfo> new_proxies,
        bool                   new_verbose
    )
        : config_file_path(argv[1]),
          proxy_collection_unique_name(argv[2]),
          output_dir_name(new_output_dir_name),
          proxies(new_proxies),
          verbose(new_verbose) {
    }
    static const char* get_help () {
        return "\
Usage:\n\
cxxplug-gen-proxy [<VERBOSE>] <config-file> <proxy-collection-unique-name> \n\
 [<output-dir-name>] [<interface-file-path] SOURCES <var-name> <header-path>...\n\
# Suggestion: use plugin collection directory name\n\
";
    }
    static bool is_argv_wrong (
        int                    argc,
        char                   **argv,
        const char             **out_dir_name,
        const char             **out_interface_file_path,
        std::vector<ProxyInfo> *proxies,
        bool                   *out_verbose
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

        // Skip <proxy-collection-unique-name>
        arg++;

        if (
            arg - argv < argc
            && strcmp("SOURCES", *arg)
            && std::filesystem::is_directory(*arg)
        ) {
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

        if (arg - argv < argc && strcmp("SOURCES", *arg)) {
            if (!std::filesystem::is_regular_file(*arg)) {
                std::cerr << "Error: file \"" << *arg
                          << "\" does not exists.\n";
                return true;
            }
            *out_interface_file_path = *arg;
            arg++;
        }

        if (arg - argv < argc && strcmp("SOURCES", *arg)) {
            std::cerr <<
                    "Error: marker \"SOURCES\" required before list start at \""
                    << *arg << "\".\n";
            return true;
        }
        else if (arg + 1 - argv >= argc) {
            std::cerr << "Error: at least 1 <var-name:header_path> pair \
required after \"SOURCES\" marker.\n";
            return true;
        }
        else {
            arg++;

            while (arg < argv + argc) {
                if (arg + 1 >= argv + argc) {
                    std::cerr << "Error: the least arguments \"" << *arg
                            << "\" misses a pair.\n";
                    return true;
                }
                else if (!std::filesystem::is_regular_file(arg[1])) {
                    std::cerr << "Error: header \"" << arg[1]
                            << "\" not found.\n";
                    return true;
                }

                proxies->push_back({
                    .var_name    = arg[0],
                    .header_path = arg[1]
                });
                arg += 2;
            }
        }

        return false;
    }
};

#endif // ARGS_HPP_INCLUDED
