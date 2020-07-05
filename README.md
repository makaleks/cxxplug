# CxxPlug

A C++ toolset for loading plugins with C ABI.

## Description

Provides:
- an interface to work with plugins whatever they are - dynamically-loaded or
  statically-linked libraries;
- code-generation tools to produce fasade for statically-linked plugins, so
  symbols redefinition conflicts are no longer a problem;
- a CMake script to produce statically-linked fasade in a single
  function call.

Requires:
- `nm`, `objcopy`, `ld` executables - implemented by GNU or LLVM, so can work
  even on Windows, despite Microsoft not providing anything similar to
  `objcopy --localize-symbol` and `ld --relocatable`.

On POSIX-compatible systems the `dlopen` requirements for dynamically-loaded
libraries location are added, so the following strategies are possible when
plugin is searched by:
- relative path - requires the resulting executable being
  launched always from the same corresponding path (not very convenient, but
  portable);
- absolute path - requires plugins to be placed by the same absolute paths (not
  portable or requires adoption for every distributive);
- executable path - when using `RPATH` method, all plugin implementations must
  be placed in the same directory as executable's, so all implementations of
  all interfaces must have unique names (portable, but messy).

Conventions:
- plugins should implement `get_implementation_uuid`, that returns pointer to a
  valid [uuid](https://en.wikipedia.org/wiki/Universally_unique_identifier);
- plugins should implement `get_implementation_name` that takes pointer to
  [iso369-3](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes) string
  and returns plugin name in the requested language, or default.

Dependencies (available as git submodules):
- [ecstrings](https://github.com/makaleks/ecstrings) - its call is written by
  code generation toos (when producing `*_interface.hpp`) to return plugin
  interface name - default or user-provided;
- [libload](https://github.com/makaleks/libload) - optional, `cxxplug` provides
  an example of dynamic library load provider based on this library.

## Build

Build:

```sh
git clone https://github.com/makaleks/cxxplug.git --recursive
cd cxxplug
cmake -B build
cmake --build build
```

Run an example:
```sh
cd build
ctest .
```

or
```sh
cd build/cxxplug/tests/overview
./overview
```

the output of the last one will be similar to
```sh
0: [Oh Plugin](Ivan Susanin)
1: [Oh Plugin](Gregor Eisenhorn)
2: [Oh Plugin](plugin_human/vasiliy_pupkin.so)

0: PluginHuman {
    src:  <built_in>

    name: "Ivan Susanin"
    uuid: cf75898-a81-47fb-afe-93ccb9c8dc8c

    first_name:    "Ivan"
    last_name:     "Susanin"
    age:           45
    work_with_int: 10 => 0
}
1: PluginHuman {
    src:  <built_in>

    name: "Gregor Eisenhorn"
    uuid: 241bee6f-5a6d-47d9-a98e-952e954c037

    first_name:    "Gregor"
    last_name:     "Eisenhorn"
    age:           42
    work_with_int: 10 => 100
}
2: PluginHuman {
    src:  "plugin_human/vasiliy_pupkin.so"

    name: "Vasiliy Puplik"
    uuid: 704599c7-1e21-4d1c-9491-a89b598a097

    first_name:    "Vasiliy"
    last_name:     "Pupkin"
    age:           30
    work_with_int: 10 => 20
}

Done
```

## Example

The following is based on [plugin_human](tests/overview/plugins/plugin_human/)
of [overview](tests/overview) example.

### CMake

1. Define plugin sources:
   ```cmake
   list(
       APPEND vasiliy_pupkin
       "vasiliy_pupkin/vasiliy_pupkin.c"
   )
   list(
       APPEND ivan_susanin
       "ivan_susanin/ivan_susanin.c"
   )
   list(
       APPEND gregor_eisenhorn
       "gregor_eisenhorn/gregor_eisenhorn.c"
   )
   ```

2. Choose which plugins should be loaded in runtime and which - must be linked:
   ```cmake
   list(
       APPEND plugins_dynamic
       "vasiliy_pupkin"
   )
   list(
       APPEND plugins_integrated
       "ivan_susanin"
       "gregor_eisenhorn"
   )
   ```

3. Build dynamic libraries as usual:
   ```cmake
   foreach( plugin_name ${plugins_dynamic} )
       add_library("${plugin_name}" MODULE "${${plugin_name}}")
       # So the output will be like "ivan_susanin_proxy.a"
       set_target_properties("${plugin_name}" PROPERTIES PREFIX "")
       target_compile_definitions("${plugin_name}" PRIVATE -DTEST_SHARED_BUILD)
   endforeach( plugin_name ${plugins_dynamic} )
   ```
4. Call [cxxplug_gen_integrated()](scripts/cxxplug_gen.cmake) to build
   statically-linked fasade.
   ```cmake
   cxxplug_gen_integrated(
       LISTS_INSIDE
       INTERFACE_CONFIG "cxxplug_interface.conf"
       INTERFACE_NAME   "plugin_human"
       IMPLEMENTATIONS  ${plugins_integrated}
   )
   # Library target object returned, must be linked with final executable
   set(plugin_human_builtin ${CXXPLUG_INTERFACE_BUILTIN} PARENT_SCOPE)
   # Include directory with generated builtin-collection and interface headers
   # returned
   include_directories("${CXXPLUG_GENERATED_INCLUDE_DIR}")

   target_link_libraries(overview ${plugin_human_builtin})
   ```

### Code

```cpp
#include <cxxplug.hpp>
#include <cxxplug/some_source_providers/filesystem_dynlib_provider.hpp>
#include <cxxplug/some_load_providers/libload_provider.hpp>

#include "cxxplug_gen/builtin/plugin_human_builtin.hpp"

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
    // Yes, you are free to define and set your own LoadProvider, SourceProvider
    // and even collection of builtin plugins - cxxplug is flexible enough
    CxxPlug<
        PluginHumanInterface,
        SourceProviderFilesystemDynLib<PluginHumanInterface>,
        LoadProviderLibload
    > plug(
        "plugin_human",
        span<PluginHumanInterface>(begin(plugin_human),end(plugin_human))
    );

    auto lst = plug.get_available();

    for (size_t i = 0; i < lst.size(); i++) {
        cout << i << ": " << lst[i].string() << "\n";
    }

    if (0 != lst.size()) {
        cout << "\n";
    }

    // In fact, it is
    // `vector<CxxPlugLoaded<PluginHumanInterface,LoadProviderLibload>> loaded;`
    // but if no wish to change template arguments someday, use
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
```

## How does generation work

[cxxplug_gen_integrated()](scripts/cxxplug_gen.cmake) was made for CMake, but
you may write your of code for any build system,
because `cxxplug_gen_integrated()` only:
1. adds plugin as static-library target,  
   for `ivan_susanin` produces `ivan_susanin.a`,
2. calls [cxxplug-gen-interface](tools/cxxplug_gen_interface.cpp) tool to
   produce interface header from plugin interface description
   [file](tests/overview/plugins/plugin_human/cxxplug_interface.conf),
   example of produced - `plugin_human_interface.hpp`;
3. calls [cxxplug-gen-proxy](tools/cxxplug_gen_proxy.cpp) tool to
   produce interface variable (whose type is defined in interface header,
   produced above) from plugin's C-implementation whith unique name,  
   for `ivan_susanin` produces `ivan_susanin_proxy_proxy.hpp`,
   `ivan_susanin_proxy.cpp` and asks CMake to build them as
   `ivan_susanin_proxy_impl.a` with a setting to be linked with
   `ivan_susanin.a`, built above;
4. calls `ld --whole-archive --relocatable` to force immediate linkage of
   `ivan_susanin_proxy_impl.a` and `ivan_susanin.a` to a single file -
   `ivan_susanin_proxy_merged.a`
5. calls [cxxplug-hide-linked-symbols](tools/cxxplug_hide_linked_symbols.cpp)
   to hide (localize) symbols, defined in some library, in other library,
   here symbols redefinition conflict goes away,  
   for `ivan_susanin` produces `ivan_susanin_proxy.a`,
6. calls
   [cxxplug-gen-collected-proxies](tools/cxxplug_gen_collected_proxies.cpp)
   to generate source files for an array, including all produced proxies,
   then adds CMake target to build it,  
   for `plugin_human` interface produces `libplugin_human_builtin.a`, which
   is returned as `${CXXPLUG_INTERFACE_BUILTIN}`, (in addition,
   `${CXXPLUG_GENERATED_INCLUDE_DIR}` with a path to
   `libplugin_human_builtin.a` header is returned)

