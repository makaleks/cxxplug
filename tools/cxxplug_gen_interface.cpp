#include <iostream>
#include <fstream>
#include <filesystem>
#include <optional>
#include <string>

#include "common/parse_config.hpp"

#include "cxxplug_gen_interface/args.hpp"
#include "cxxplug_gen_interface/get_filepath.hpp"
#include "cxxplug_gen_interface/gen_interface.hpp"

using namespace std;

int main (int argc, char **argv) {
    bool       is_verbose               = false;
    const char *output_dir_or_file_name = "./";

    if (
        Args::is_argv_wrong(argc, argv, &output_dir_or_file_name, &is_verbose)
    ) {
        cout << Args::get_help();
        return -1;
    };
    Args args(argv + is_verbose, output_dir_or_file_name, is_verbose);

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

        string interface_filepath = get_interface_filepath(
            config_file_data, args.output_dir_or_file_name
        );
        gen_interface(interface_filepath, config_file_data);

        cout << interface_filepath;
    }
    catch (const fstream::failure &e) {
        cerr << "Could not proceed file: " << e.what() << "\n";
        return -1;
    }
    catch (const filesystem::filesystem_error &e) {
        cerr << "Could not proceed filesystem operations: " << e.what() << "\n";
        return -1;
    }

    return 0;
}
