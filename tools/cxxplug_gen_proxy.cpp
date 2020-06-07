#include <fstream>
#include <optional>

#include "common/parse_config.hpp"
#include "cxxplug_gen_proxy/args.hpp"
#include "cxxplug_gen_proxy/gen_proxy.hpp"

using namespace std;

int main (int argc, char **argv) {
    bool       is_verbose               = false;
    const char *output_dir_name         = "./";
    const char *interface_file_path_arg = nullptr;

    if (
        Args::is_argv_wrong(
            argc, argv, &output_dir_name, &interface_file_path_arg, &is_verbose
        )
    ) {
        cout << Args::get_help();
        return -1;
    };
    Args args(argv + is_verbose, output_dir_name, is_verbose);

    if (args.verbose) {
        args.print();
    }

    try {
        Parsed config_file_data = Parsed::get_parsed_config(
            args.config_file_path
        );

        if (args.verbose) {
            config_file_data.print();
        }

        string      interface_file_path_default;
        string_view interface_file_path;
        if (interface_file_path_arg) {
            interface_file_path = interface_file_path_arg;
        }
        else {
            interface_file_path_default
                    = string(config_file_data.interface_name)
                    + "_interface.hpp";
            interface_file_path = interface_file_path_default;
        }

        ProxyGenerationResult result = generateProxyFiles(
            args, config_file_data, interface_file_path
        );

        cout << result.header << "\n" << result.source;
    }
    catch (const ifstream::failure &e) {
        cerr << "Could not parse config file\n";
        return -1;
    }


    return 0;
}
