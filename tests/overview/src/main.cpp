#include <iostream>
#include <string_view>
#include <vector>
#include <type_traits>

using namespace std;

#include <cxxplug.hpp>
#include <cxxplug/some_source_providers/filesystem_dynlib_provider.hpp>
#include <cxxplug/some_load_providers/libload_provider.hpp>

#include "cxxplug_gen/builtin/plugin_human_builtin.hpp"

#include <filesystem>

template <typename PlugLoaded>
void print_plugin_human (PlugLoaded &loaded) {
    char lang[] = "eng";
    int  value = 10;
    cout << "\
PluginHuman {\n\
    src:  " <<
        (
            loaded.is_builtin
            ? "<built_in>"
            : (string("\"") + loaded.source_name + "\"")
        ) << "\n\
\n\
    name: \"" << loaded.impl.get_implementation_name(lang) << "\"\n\
    uuid: "   << *(Uuid*)loaded.impl.get_implementation_id() << "\n\
\n\
    first_name:    \"" << loaded.impl.get_first_name() << "\"\n\
    last_name:     \"" << loaded.impl.get_last_name() << "\"\n\
    age:           "   << std::dec << (unsigned)loaded.impl.get_age() << "\n\
    work_with_int: "   << value;
    loaded.impl.work_with_int(&value);
    cout << " => " << value << "\n\
}\n";
}

int main () {
    cout << filesystem::current_path() << '\n';

    // Yes, you are free to define and set your own LoadProvider, SourceProvider
    // and even collection of builtin plugins - cxxplug flexible enough
    CxxPlug<
        PluginHumanInterface,
        SourceProviderFilesystemDynLib<PluginHumanInterface>,
        LoadProviderLibload
    > plug(
        "plugin_human",
        // This array was generated using `cxxplug-gen-collected-proxies` tool
        span<PluginHumanInterface>(begin(plugin_human),end(plugin_human))
    );

    auto lst = plug.get_available();

    for (size_t i = 0; i < lst.size(); i++) {
        cout << i << ": " << lst[i].string() << "\n";
    }

    if (0 != lst.size()) {
        cout << "\n";
    }

    vector<result_of<
        decltype(&decltype(plug)::load)(decltype(plug), decltype(lst[0]))
    >::type> loaded;
    for (size_t i = 0; i < lst.size(); i++) {
        loaded.emplace_back(plug.load(lst[i]));
        cout << i << ": ";
        print_plugin_human(loaded[i]);
    }

    cout << "\nDone\n";
    return 0;
}
