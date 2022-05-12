#pragma once

#include <cstdint>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    // TIFF file information (readonly)
    struct DermTIFF {
        static constexpr uint32_t MaxWidth = 16384, MaxHeight = 16384;

        const uint32_t width, height;
        const uint16_t pages;
        const uint16_t photoMetric;
        const uint16_t samplesPerPixel;
        const int64_t scanlineSize;

        const bool isValid;

        explicit DermTIFF(const std::string& path);

        explicit DermTIFF(TIFF* const tiff);
    };
}
