#ifndef CXX_PLUG_LIBLOAD_PROVIDER_HPP_INCLUDED
#define CXX_PLUG_LIBLOAD_PROVIDER_HPP_INCLUDED 0

//#include "cxxplug/cxxplugloadprovider.hpp"
#include "cxxplug/uuid.hpp"

#include "libload.h"

#include <cstring>
#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <iostream>

//constexpr Uuid LOAD_PROVIDER_UUID_LIBLOAD
//        = Uuid(std::array<char,37>{{"621ede07-73ea-4a97-b36f-94f967065e18"}});

//class UuidDefined<LOAD_PROVIDER_UUID_LIBLOAD> {
class LoadProviderLibload {
    LibloadHandler handler_;
    bool           is_inited_ = false;
public:
    LoadProviderLibload () = default;
    //UuidDefined<LOAD_PROVIDED_UUID_LIBLOAD> () = default;
    //UuidDefined<LOAD_PROVIDED_UUID_LIBLOAD> (std::string editable_filepath) {
    LoadProviderLibload  (std::string editable_filepath) : is_inited_(true) {
        LIBLOAD_STATUS status = LIBLOAD_SUCCESS;
        if (4 <= editable_filepath.length()) {
            auto extension_start = std::prev(editable_filepath.end(), 4);
            if (
                ".dll" ==
                std::string_view(extension_start, editable_filepath.end())
            ) {
                // libload requires ".so" extension
                editable_filepath.replace(
                    extension_start, editable_filepath.end(), ".so"
                );
            }
        }
        //std::cout << "<loading...";
        status = libloadfile(
            // libload must return unmodified version
            &handler_, const_cast<char*>(editable_filepath.c_str())
        );
        if (status) {
            throw std::runtime_error("Dynamic library not found?");
        }
        //std::cout << "done>\n";
    }
    int load_symbol(const char *symbol_name, void (**out_symbol)()) {
        LIBLOAD_STATUS status = LIBLOAD_SUCCESS;
        status = libloadsymbol(&handler_, out_symbol, (char*)symbol_name);
        if (status) {
            return -1;
        }
        return 0;
    }
    //~UuidDefined<LOAD_PROVIDED_UUID_LIBLOAD> () {
    ~LoadProviderLibload () {
        if (is_inited_) {
            LIBLOAD_STATUS status = LIBLOAD_SUCCESS;
            //std::cout << "<unloading...";
            status = libloadfinish(&handler_);
            if (status) {
                std::cerr << "Error on libloadfinish\n";
            }
            //std::cout << "done>\n";
        }
    }
};

#endif
