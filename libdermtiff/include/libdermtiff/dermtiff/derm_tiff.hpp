#pragma once

#include <cstdint>
#include <string>

typedef struct tiff TIFF;

namespace ldt {
    // TIFF file information (readonly)
    struct DermTIFF {
        static constexpr uint32_t max_width = 16384, max_height = 16384;

        bool is_valid;

        uint16_t page_count, layer_count;
        uint32_t width, height;
        uint16_t bits_per_sample;
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
        uint16_t extra_samples;

        uint16_t photometric;
        uint16_t planar_config;
        uint16_t samples_per_pixel;
        uint32_t sub_file_type;
    };
}
