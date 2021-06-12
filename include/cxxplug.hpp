#ifndef CXXPLUG_HPP_INCLUDED
#define CXXPLUG_HPP_INCLUDED 0

#include <vector>
#include <span>

#include <tuple>
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <ostream>

#include "cxxplug/uuid.hpp"
#include "cxxplug/i18n_provided.hpp"
#include "cxxplug/load_candidate.hpp"
#include "cxxplug/plug_loaded.hpp"

template <
    typename PluginInterface,
    typename SourceProvider,
    typename LoadProvider
>
class CxxPlug {
    SourceProvider               source_provider_;
    std::span<PluginInterface>   builtin_plugins_;
    std::vector<PluginInterface> loaded_plugins_;
public:
    CxxPlug(
        std::string search_path,
        std::span<PluginInterface> builtin_plugins
                = std::span<PluginInterface>()
    ) : source_provider_(search_path), builtin_plugins_(builtin_plugins) {
    }
    LoadCandidateContainer<PluginInterface> get_available() {
        auto v = source_provider_.list_load_sources();
        return LoadCandidateContainer<PluginInterface>(
            builtin_plugins_, std::move(v)
        );
    }
    CxxPlugLoaded<PluginInterface,LoadProvider> load(
        const CxxPlugLoadCandidate<PluginInterface> target
    ) {
        if (target.builtin_ptr) {
            return CxxPlugLoaded<PluginInterface,LoadProvider>(
                *target.builtin_ptr
            );
        }
        else {
            return CxxPlugLoaded<PluginInterface,LoadProvider>(
                target.source
            );
        }
    }
};

#endif
