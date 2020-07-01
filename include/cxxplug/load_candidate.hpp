#ifndef CXX_PLUG_LOAD_CANDIDATE_HPP
#define CXX_PLUG_LOAD_CANDIDATE_HPP

#include <string>

//#include "load_provider.hpp"

template <typename PluginInterface>
struct CxxPlugLoadCandidate {
    std::string     source;
    PluginInterface *builtin_ptr = nullptr;
    CxxPlugLoadCandidate(std::string new_source) : source(new_source) {
    }
    CxxPlugLoadCandidate(PluginInterface *ptr) : builtin_ptr(ptr) {
    }
    std::string string() {
        char lang[] = "eng";
        std::string base = std::string("[")
                + PluginInterface::get_interface_name(reinterpret_cast<ECStringType*>(lang)) + "](";
        if (nullptr == builtin_ptr) {
            return base + source + ")";
        }
        else {
            return base + builtin_ptr->get_implementation_name(lang) + ")";
        }
    }
};

#include <span>
#include <vector>
#include <initializer_list>

template <typename PluginInterface>
class LoadCandidateContainer {
    std::span<PluginInterface>                         span_;
    std::vector<CxxPlugLoadCandidate<PluginInterface>> vec_;
public:
    LoadCandidateContainer(
        std::span<PluginInterface> s,
        std::vector<CxxPlugLoadCandidate<PluginInterface>> v
    )
        : span_(s), vec_(v) {}
    CxxPlugLoadCandidate<PluginInterface> operator[] (size_t n) {
        if (n < span_.size()) {
            return CxxPlugLoadCandidate<PluginInterface>(&span_[n]);
        }
        else {
            n -= span_.size();
            return vec_[n];
        }
    }
    size_t size() {
        return span_.size() + vec_.size();
    }
};

#endif // CXX_PLUG_LOAD_CANDIDATE_HPP
