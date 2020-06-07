#ifndef PREDEFINED_SYMBOLS_HPP_INCLUDED
#define PREDEFINED_SYMBOLS_HPP_INCLUDED 0

extern const char* PredefinedSymbols[2];

enum {
    SYMBOL_GET_INTERFACE_ID,
    SYMBOL_GET_IMPLEMENTATION_ID,
    // TODO: preserve hand-written i18n names in
    // cxxplug_gen_interface & cxxplug_gen_proxy
    //SYMBOL_GET_INTERFACE_NAME
    //SYMBOL_GET_IMPLEMENTATION_NAME
};

#endif // PREDEFINED_SYMBOLS_HPP_INCLUDED
