#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace ldt {
    struct Pencil {
        std::string name = "";
        uint8_t r = 0, g = 0, b = 0, a = 0;

        static std::optional<Pencil> Parse(const std::string& str);

        std::optional<std::string> toString() const;

        bool operator==(const Pencil& other) const {
            return name == other.name && r == other.r && g == other.g && b == other.b;
        }

        bool operator!=(const Pencil& other) const {
            return !(*this == other);
        }
    };
}
