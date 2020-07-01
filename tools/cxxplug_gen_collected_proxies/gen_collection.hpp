#ifndef GEN_COLLECTION_HPP_INCLUDED
#define GEN_COLLECTION_HPP_INCLUDED 0

#include <array>
#include <string>
#include <string_view>

#include "args.hpp"
#include "../common/parse_config.hpp"

struct CollectionGenerationResult {
    std::string header;
    std::string source;
};

CollectionGenerationResult
generateCollectionFiles (
    const Args &args, const Parsed &config, std::string_view interface_file_path
);

#endif // GEN_COLLECTION_HPP_INCLUDED
