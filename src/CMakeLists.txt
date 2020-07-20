#add_library(cxxplug STATIC)
add_library(cxxplug INTERFACE)
target_link_libraries(cxxplug INTERFACE ${ecstrings} ${libload})

set(cxxplug ${cxxplug} PARENT_SCOPE)
