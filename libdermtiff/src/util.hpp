#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    namespace util {
        // T: uint16_t and uint32_t are implemented.
        // If could not get the field, 0 will be returned.
        template <typename T>
        T GetField(TIFF* const tiff, uint32_t tag) noexcept;

        template <typename T>
        std::optional<T> GetFieldOpt(TIFF* const tiff, uint32_t tag) noexcept {
            if (tiff == nullptr) {
                return std::nullopt;
            } else {
                if (T value; TIFFGetField(tiff, tag, &value) == 1) {
                    return value;
                } else {
                    return std::nullopt;
                }
            }
        }

        std::shared_ptr<TIFF> SafeTIFFOpen(std::string_view path, const char* mode) noexcept;

        std::shared_ptr<TIFF> SafeTIFFOpenW(std::wstring_view path, const char* mode) noexcept;
    }
}
