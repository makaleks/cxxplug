#ifndef CXX_PLUG_LOADED_HPP
#define CXX_PLUG_LOADED_HPP 0

#include <stdexcept>
#include <memory>

//#include "cxxplug/implementation.hpp"
#include "cxxplug/uuid.hpp"
#include <iostream>

template <typename PlugInterface, typename LoadProvider>
class CxxPlugLoaded {
    std::shared_ptr<LoadProvider> load_provider_;

    template <typename F>
    void load_symbol(const char *name, F &f) {
        if (load_provider_->load_symbol(name, reinterpret_cast<void(**)()>(&f))) {
            throw std::domain_error(name);
        }
    }
public:
    PlugInterface impl;
    const std::string source_name;
    const bool is_builtin;

    CxxPlugLoaded(
        PlugInterface builtin_impl
    ) noexcept : impl(builtin_impl), is_builtin(true) {
    };
    CxxPlugLoaded(
        std::string nsource_name
    ) :
        load_provider_(std::make_shared<LoadProvider>(nsource_name)),
        source_name(nsource_name),
        is_builtin(false)
    {
        PlugInterface result;
        std::apply(
            [this](
                const void* (**implementation_id_ptr)(),
                const char* (**implementation_name_ptr)(const char[4]),
                auto... args
            ) -> void {
                const char *implementation_id_func   = "get_implementation_id";
                const char *implementation_name_func = "get_implementation_name";
                if (
                    this->load_provider_->load_symbol(
                        implementation_id_func, reinterpret_cast<void(**)()>(implementation_id_ptr)
                    )
                ) {
                    throw std::domain_error(implementation_id_func);
                }
                else if (
                    this->load_provider_->load_symbol(
                        implementation_name_func, reinterpret_cast<void(**)()>(implementation_name_ptr)
                    )
                ) {
                    throw std::domain_error(implementation_name_func);
                }
                (this->load_symbol(std::get<0>(args), std::get<1>(args)), ...);
            },
            //std::make_tuple(&result.get_implementation_id, &result.get_implementation_name)
            std::tuple_cat(
                std::make_tuple(&result.get_implementation_id, &result.get_implementation_name),
                result.get_tuple_map()
            )
        );
        impl = result;
    };
};

#endif // CXX_PLUG_LOADED_HPP
