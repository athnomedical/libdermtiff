#pragma once

#include <cstdint>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    // TIFF file information (readonly)
    struct DermTIFF {
        static constexpr uint32_t MaxWidth = 16384, MaxHeight = 16384;

        const uint16_t pageCount, layerCount;
        const uint32_t width, height;
        const bool isValid;

        explicit DermTIFF(const std::string& path);

        explicit DermTIFF(TIFF* const tiff);
    };
}
