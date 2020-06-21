#ifndef CXX_PLUG_INTERNATIONALIZATION_PROVIDED
#define CXX_PLUG_INTERNATIONALIZATION_PROVIDED 0

#include "ecstrings.h"

#include <string>
#include <string_view>

struct ProvidedString {
    ECStringType lang_iso369_3;
    std::string  str;
};

struct ProvidedStringView {
    ECStringType     lang_iso369_3;
    std::string_view str;
};

#endif // CXX_PLUG_INTERNATIONALIZATION_PROVIDED
