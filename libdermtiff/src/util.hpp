#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    namespace util {
        template <typename T>
        T GetField(TIFF* const tiff, uint32_t tag) {
            if (tiff == nullptr) {
                return T();
            } else {
                T value;
                TIFFGetField(tiff, tag, &value);
                return value;
            }
        }

        template <typename T>
        std::optional<T> GetFieldOpt(TIFF* const tiff, uint32_t tag) {
            if (tiff == nullptr) {
                return std::nullopt;
            } else {
                if (T value; TIFFGetField(tiff, tag, &value) != 1) {
                    return std::nullopt;
                } else {
                    return value;
                }
            }
        }

        std::shared_ptr<TIFF> SafeTIFFOpen(const std::string& path, const char* mode);
    }
}
