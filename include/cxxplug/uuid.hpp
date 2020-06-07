#ifndef CXX_PLUG_UUID_HPP
#define CXX_PLUG_UUID_HPP 0

#include <cstdint>
#include <array>
#include <stdexcept>

struct Uuid {
    std::uint8_t     data[16] = {0};
    std::array<char, 37> name = {0};
    constexpr Uuid (const char input[37]) {
        for (unsigned i = 0; i < 37; i++) {
            name[i] = input[i];
        }
        // See https://en.wikipedia.org/wiki/Universally_unique_identifier
        // Correct input example
        // 123e4567-e89b-12d3-a456-426655440000
        // (8-4-4-4-12)
        auto is_wrong = [](const char input [37]) -> bool {
            if ('\0' != input[36]) {
                return false;
            }
            for (size_t i = 0; i < 37; i++) {
                if (8 == i || 13 == i || 18 == i || 23 == i) {
                    if ('-' != input[i]) {
                        return false;
                    }
                }
                else {
                    if (
                        input[i] < '0'
                        || ('9' < input[i] && input[i] < 'a')
                        || 'z' < input[i]
                    ) {
                        return false;
                    }
                }
            }
            return true;
        };
        if (is_wrong(input)) {
            throw std::invalid_argument(input);
        }
        for (unsigned i = 0, half_byte_i = 0; i < 36; i++) { // still constexpr
            if (8 != i && 13 != i && 18 != i && 23 != i) {
                data[half_byte_i / 2] =
                        (data[half_byte_i / 2] << 4)
                        | (input[i] <= '9'
                            ? input[i] - '0'
                            : input[i] - 'a' + 10
                        );
                half_byte_i++;
            }
        }
    }
};

template <Uuid uuid>
struct UuidDefined;

#endif // CXX_PLUG_UUID_HPP
