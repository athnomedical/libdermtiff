#pragma once

#include <cstdint>
#include <string>

#define EXPORT extern "C" __declspec(dllexport)
#define STDCALL __stdcall

typedef struct tiff TIFF;

namespace ldt {
    // TIFF file information (readonly)
    struct DermTIFF {
        static constexpr uint32_t MaxWidth = 16384, MaxHeight = 16384;

        uint16_t pageCount, layerCount;
        uint32_t width, height;
        bool isOpened, isValid;
        uint16_t bitsPerSample;
        uint16_t compression;

        // Sum of EXTRASAMPLE_UNSPECIFIED:2, EXTRASAMPLE_ASSOCALPHA:3 and EXTRASAMPLE_UNASSALPHA:4.
        // 0: No extra samples
        // 2: EXTRASAMPLE_UNSPECIFIED
        // 3: EXTRASAMPLE_ASSOCALPHA
        // 4: EXTRASAMPLE_UNASSALPHA
        // 5: EXTRASAMPLE_UNSPECIFIED + EXTRASAMPLE_ASSOCALPHA
        // 6: EXTRASAMPLE_UNSPECIFIED + EXTRASAMPLE_UNASSALPHA
        // 7: EXTRASAMPLE_ASSOCALPHA + EXTRASAMPLE_UNASSALPHA
        // 9: EXTRASAMPLE_UNSPECIFIED + EXTRASAMPLE_ASSOCALPHA + EXTRASAMPLE_UNASSALPHA
        uint16_t extraSamples;

        uint16_t photometric;
        uint16_t planarConfig;
        uint16_t samplesPerPixel;
        uint32_t subFileType;
    };

    DermTIFF OpenDermTIFF(TIFF* const tiff);

    DermTIFF OpenDermTIFF(std::string_view path);

#ifdef _WIN32
    DermTIFF OpenDermTIFF(std::wstring_view path);
#endif
}
