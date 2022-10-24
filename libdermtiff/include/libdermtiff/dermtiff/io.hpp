#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "pencil.hpp"
#include "type.hpp"

namespace ldt::io {
    enum class Orientation {
        TOPLEFT = 1,
        TOPRIGHT,
        BOTRIGHT,
        BOTLEFT,
        LEFTTOP,
        RIGHTTOP,
        RIGHTBOT,
        LEFTBOT,
    };

    // Try to open tiff file
    DermTIFF OpenTIFF(const char* filepath);
    DermTIFF OpenTIFFW(const wchar_t* filepath);

    // Read the specific page
    // - Page 0		: Original image
    // - Page 1...	: Layers
    // Raster should be uint32[width * height]
    bool ReadPage(const char* filepath,
                  uint16_t page,
                  uint32_t* raster,
                  Pencil* pencil,
                  Orientation orientation = Orientation::LEFTTOP);
    bool ReadPageW(const wchar_t* filepath,
                   uint16_t page,
                   uint32_t* raster,
                   Pencil* pencil,
                   Orientation orientation = Orientation::LEFTTOP);

    // Read the original image of dermtiff
    // Raster should be uint32[width * height]
    bool ReadOriginalImage(const char* filepath, uint32_t* raster, Orientation orientation = Orientation::LEFTTOP);
    bool ReadOriginalImageW(const wchar_t* filepath, uint32_t* raster, Orientation orientation = Orientation::LEFTTOP);

    // Read the specific layer of dermtiff
    // Raster should be uint32[width * height]
    bool ReadLayer(const char* filepath,
                   uint16_t layerIndex,
                   uint32_t* raster,
                   Pencil* pencil,
                   Orientation orientation = Orientation::LEFTTOP);
    bool ReadLayerW(const wchar_t* filepath,
                    uint16_t layerIndex,
                    uint32_t* raster,
                    Pencil* pencil,
                    Orientation orientation = Orientation::LEFTTOP);

    // Raster should be uint32[width * height]
    // Samples per pixel is 4
    bool WriteTIFF(const char* filepath,
                   uint16_t layerCount,
                   uint32_t width,
                   uint32_t height,
                   const uint32_t* const* const rasters,
                   const Pencil* const pencils);
    bool WriteTIFFW(const wchar_t* filepath,
                    uint16_t layerCount,
                    uint32_t width,
                    uint32_t height,
                    const uint32_t* const* const rasters,
                    const Pencil* const pencils);
}
