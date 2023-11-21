#pragma once

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

namespace ldt {
    struct Pencil {
        static const uint8_t max_name_length = 64;

        char name[max_name_length + 1] = "\0";
        uint8_t r                      = 0;
        uint8_t g                      = 0;
        uint8_t b                      = 0;
        uint8_t a                      = 255;

        Pencil() = default;

        Pencil(std::string_view _name);

        Pencil(std::string_view _name, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255);

        static std::optional<Pencil> parse(std::string_view str);

        bool set_name(std::string_view _name);

        std::optional<std::string> to_string() const;

        bool operator==(const Pencil& other) const noexcept {
            return strcmp(name, other.name) == 0 && r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(const Pencil& other) const noexcept {
            return !(*this == other);
        }
    };
}
