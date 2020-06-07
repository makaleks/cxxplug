#include "hide_symbols.hpp"

#include <string>
#include <cstdlib>

using namespace std;

void hide_symbols (
    const string_view                 &command_objcopy,
    const string_view                 &lib_path,
    const std::list<std::string_view> symbols
) {
    if (symbols.empty()) {
        return;
    }

    string command = command_objcopy.data();

    for (auto &s : symbols) {
        command.append(string(" --localize-symbol ") + string(s));
    }

    command.append(string(" ") + lib_path.data());

    system(command.c_str());
}
