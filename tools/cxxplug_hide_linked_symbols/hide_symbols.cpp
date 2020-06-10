#include "hide_symbols.hpp"

#include <string>
#include <cstdlib>

using namespace std;

void hide_symbols (
    const string_view            &command_objcopy,
    const string_view            &source_lib_path,
    const string_view            &destination_lib_path,
    const std::list<std::string> symbols
) {
    if (symbols.empty()) {
        return;
    }

    string command = command_objcopy.data();

    for (auto &s : symbols) {
        command.append(string(" --localize-symbol ") + s);
    }

    command.append(
        string(" ") + source_lib_path.data()
        + " " + destination_lib_path.data()
    );

    system(command.c_str());
}
