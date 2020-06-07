#include <fstream>
#include <iostream>

#include "common/parse_config.hpp"
#include "cxxplug_print_unwanted_symbols/args.hpp"
#include "cxxplug_print_unwanted_symbols/get_symbols.hpp"
#include "common/symbols_serialization.hpp"

using namespace std;

int main (int argc, char **argv) {
    bool       is_verbose  = false;
    const char *command_nm = "nm";

    if (Args::is_argv_wrong(argc, argv, &command_nm, &is_verbose)) {
        cout << Args::get_help();
        return -1;
    };
    Args args(argv + is_verbose, command_nm, is_verbose);

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

        list<string> symbols = get_all_symbols(args);

        if (args.verbose) {
            print_all_symbols(symbols, config_file_data);
        }

        //remove_unwanted_symbols(&symbols, config_file_data);

        string result = serialize_symbols(symbols.begin(), symbols.end());
        cout << result;
    }
    catch (const ifstream::failure &e) {
        cerr << "Could not parse config file\n";
        return -1;
    }

    cout << "\n";

    return 0;
}
