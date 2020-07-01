#ifndef CXX_PLUG_UUID_HPP
#define CXX_PLUG_UUID_HPP 0

#include <cstdint>
#include <array>
#include <stdexcept>
#include <ostream>

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

inline std::ostream& operator<<(std::ostream &stream, const Uuid &uuid) {
    const std::uint8_t* data = uuid.data;
    stream << std::hex << +data[0] << +data[1] << +data[2] << +data[3]
                << '-' << +data[4] << +data[5]
                << '-' << +data[6] << +data[7]
                << '-' << +data[8] << +data[9]
                << '-' << +data[10] << +data[11] << +data[12]
                       << +data[13] << +data[14] << +data[15];
    return stream;
}

constexpr bool operator== (const Uuid &uuid1, const Uuid &uuid2) {
    for (unsigned i = 0; i < 16; i++) {
        if (uuid1.data[i] != uuid2.data[i]) {
            return false;
        }
    }
    return true;
}

template <Uuid uuid>
struct UuidDefined;

#endif // CXX_PLUG_UUID_HPP
