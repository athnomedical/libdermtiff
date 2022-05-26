#pragma once

#include <cstdint>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    // TIFF file information (readonly)
    struct DermTIFF {
        struct Detail {
            uint16_t bitsPerSample;
            uint16_t compression;
            uint16_t extraSamples;
            uint16_t photometric;
            uint16_t planarConfig;
            uint16_t samplesPerPixel;
            uint32_t subFileType;
        };

        static constexpr uint32_t MaxWidth = 16384, MaxHeight = 16384;

        const uint16_t pageCount, layerCount;
        const uint32_t width, height;
        const bool isOpened, isValid;
        const Detail detail;

        explicit DermTIFF(const std::string& path);

        explicit DermTIFF(TIFF* const tiff);
    };
}
