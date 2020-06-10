#include <fstream>
#include <iostream>
#include <stdexcept>

#include "cxxplug_hide_linked_symbols/args.hpp"
#include "cxxplug_hide_linked_symbols/hide_symbols.hpp"
#include "cxxplug_hide_linked_symbols/get_symbols.hpp"

using namespace std;

int main (int argc, char **argv) {
    bool       is_verbose            = false;
    const char *source_lib_path      = nullptr;
    const char *destination_lib_path = nullptr;
    const char *command_nm           = "nm";
    const char *command_objcopy      = "objcopy";

    if (
        Args::is_argv_wrong(
            argc,
            argv,
            &source_lib_path,
            &destination_lib_path,
            &command_nm,
            &command_objcopy,
            &is_verbose
        )
    ) {
        cout << Args::get_help();
        return -1;
    };
    if (!destination_lib_path) {
        destination_lib_path = source_lib_path;
    }
    Args args(
        source_lib_path,
        destination_lib_path,
        command_nm,
        command_objcopy,
        is_verbose
    );

    if (args.verbose) {
        args.print();
    }

    try {
        list<string> symbols = get_all_symbols(
            args.command_nm, args.source_lib_static_path
        );

        if (args.verbose) {
            print_all_symbols(symbols);
        }

        hide_symbols(
            args.command_objcopy,
            args.source_lib_static_path,
            args.destination_lib_static_path,
            symbols
        );
    }
    catch (const exception &e) {
        cerr << "Failed: " << e.what() << "\n";
        return -1;
    }

    cout << "\n";

    return 0;
}
