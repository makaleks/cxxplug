#include <fstream>
#include <iostream>

#include "common/parse_config.hpp"
#include "cxxplug_hide_symbols/args.hpp"
#include "cxxplug_hide_symbols/hide_symbols.hpp"
#include "common/symbols_serialization.hpp"

using namespace std;

int main (int argc, char **argv) {
    bool             is_verbose       = false;
    const char       *command_objcopy = "objcopy";
    list<string_view> symbols;

    if (
        Args::is_argv_wrong(argc, argv, &command_objcopy, &symbols, &is_verbose)
    ) {
        cout << Args::get_help();
        return -1;
    };
    Args args(argv + is_verbose, command_objcopy, move(symbols), is_verbose);

    if (args.verbose) {
        args.print();
    }

    hide_symbols(args.command_objcopy, args.lib_static_path, args.symbols);

    return 0;
}
