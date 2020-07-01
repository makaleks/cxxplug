#ifndef CXXPLUG_FILESYSTEM_DYNLIB_PROVIDER_HPP
#define CXXPLUG_FILESYSTEM_DYNLIB_PROVIDER_HPP 0

#include "cxxplug/uuid.hpp"
#include "cxxplug/load_candidate.hpp"

#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

//constexpr Uuid SOURCE_PROVIDER_UUID_FILESYSTEM_DYNLIB
//        = Uuid(std::array<char,37>{{"7df625c5-daa3-4bc2-bb0a-c6457e7784b3"}});

//class UuidDefined<SOURCE_PROVIDER_UUID_FILESYSTEM_DYNLIB> {
template <typename PluginInterface>
class SourceProviderFilesystemDynLib {
    std::string base_dirpath_;

    bool is_dynlib_extension (const std::filesystem::path &path) {
        auto extension = path.extension();
        return ".so" == extension || ".dll" == extension;
    }
    static bool loaded_candidate_comp (
        const CxxPlugLoadCandidate<PluginInterface> &right,
        const CxxPlugLoadCandidate<PluginInterface> &left
    ) {
        return right.source < left.source;
    }
public:
    //UuidDefined<SOURCE_PROVIDER_UUID_FILESYSTEM_DYNLIB> (
    SourceProviderFilesystemDynLib (
        std::string search_dirpath
    )
      : base_dirpath_(search_dirpath)
    {
        if (!std::filesystem::is_directory(base_dirpath_)) {
            throw std::filesystem::filesystem_error(
                "Provided path is not a directory", std::error_code()
            );
        }
    }
    void change_base_dirpath_(std::string &next_search_dirpath) {
        base_dirpath_ = next_search_dirpath;
    }
    bool is_loadable_file (const std::filesystem::path &path) {
        if (!std::filesystem::is_regular_file(path)) {
            return false;
        }
        else {
            return is_dynlib_extension(path);
        }
    }
    std::vector<CxxPlugLoadCandidate<PluginInterface>> list_load_sources() {
        std::vector<CxxPlugLoadCandidate<PluginInterface>> result;
        for (auto &entry : std::filesystem::directory_iterator(base_dirpath_)) {
            if (is_loadable_file(entry.path())) {
                auto str = entry.path().string();
                result.insert(
                    std::upper_bound(
                        result.begin(), result.end(), str, loaded_candidate_comp
                    ),
                    std::move(str)
                );
            }
        }
        return result;
    }
};

#endif // CXXPLUG_FILESYSTEM_DYNLIB_PROVIDER_HPP
