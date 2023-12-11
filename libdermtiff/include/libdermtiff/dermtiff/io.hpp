#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "common.hpp"
#include "derm_tiff.hpp"
#include "pencil.hpp"

namespace ldt::io {
    enum class Orientation {
        TopLeft = 1,
        TopRight,
        BottomRight,
        BottomLeft,
        LeftTop,
        RightTop,
        RightBottom,
        LeftBottom,
    };

    // Try to open tiff file
    EXPORT DermTIFF STDCALL open_tiff(const char* filepath);

    // Read the specific page
    // - Page 0		: Original image
    // - Page 1...	: Layers
    // Raster should be uint32[width * height]
    EXPORT bool STDCALL read_page(const char* filepath,
                                  uint16_t page,
                                  uint32_t* raster,
                                  Pencil* pencil,
                                  Orientation orientation = Orientation::LeftTop);

    // Read the original image of dermtiff
    // Raster should be uint32[width * height]
    EXPORT bool STDCALL read_original_image(const char* filepath,
                                            uint32_t* raster,
                                            Orientation orientation = Orientation::LeftTop);

    // Read the specific layer of dermtiff
    // Raster should be uint32[width * height]
    EXPORT bool STDCALL read_layer(const char* filepath,
                                   uint16_t layer_index,
                                   uint32_t* raster,
                                   Pencil* pencil,
                                   Orientation orientation = Orientation::LeftTop);

    // Raster should be uint32[width * height]
    // Samples per pixel is 4
    EXPORT bool STDCALL write_tiff(const char* filepath,
                                   uint16_t layer_count,
                                   uint32_t width,
                                   uint32_t height,
                                   const uint32_t* const* const rasters,
                                   const Pencil* const pencils);

#ifdef _WIN32
    EXPORT DermTIFF STDCALL open_tiff_w(const wchar_t* filepath);

    EXPORT bool STDCALL read_page_w(const wchar_t* filepath,
                                    uint16_t page,
                                    uint32_t* raster,
                                    Pencil* pencil,
                                    Orientation orientation = Orientation::LeftTop);
    EXPORT bool STDCALL read_original_image_w(const wchar_t* filepath,
                                              uint32_t* raster,
                                              Orientation orientation = Orientation::LeftTop);

    EXPORT bool STDCALL read_layer_w(const wchar_t* filepath,
                                     uint16_t layer_index,
                                     uint32_t* raster,
                                     Pencil* pencil,
                                     Orientation orientation = Orientation::LeftTop);

    EXPORT bool STDCALL write_tiff_w(const wchar_t* filepath,
                                     uint16_t layer_count,
                                     uint32_t width,
                                     uint32_t height,
                                     const uint32_t* const* const rasters,
                                     const Pencil* const pencils);
#endif
}
