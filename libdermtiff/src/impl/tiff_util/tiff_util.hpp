#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    namespace tiff_util {
        // T: uint16_t and uint32_t are implemented.
        // If could not get the field, 0 will be returned.
        template <typename T>
        T get_field(TIFF* const tiff, uint32_t tag) noexcept;

        template <typename T>
        std::optional<T> get_field_opt(TIFF* const tiff, uint32_t tag) noexcept {
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

        std::shared_ptr<TIFF> open_tiff(std::string_view path, const char* mode) noexcept;

#ifdef _WIN32
        std::shared_ptr<TIFF> open_tiff_w(std::wstring_view path, const char* mode) noexcept;
#endif
    }
}
