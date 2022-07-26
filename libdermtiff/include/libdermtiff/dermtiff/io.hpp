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
    DermTIFF OpenTIFF(std::string_view filepath);

    // Read the specific page
    // - Page 0		: Original image
    // - Page 1...	: Layers
    // Raster should be uint32[width * height]
    bool ReadPage(std::string_view filepath,
                  uint16_t page,
                  uint32_t* raster,
                  Pencil* pencil,
                  Orientation orientation = Orientation::LEFTTOP);

    // Read the original image of dermtiff
    // Raster should be uint32[width * height]
    bool ReadOriginalImage(std::string_view filepath, uint32_t* raster, Orientation orientation = Orientation::LEFTTOP);

    // Read the specific layer of dermtiff
    // Raster should be uint32[width * height]
    bool ReadLayer(std::string_view filepath,
                   uint16_t layerIndex,
                   uint32_t* raster,
                   Pencil* pencil,
                   Orientation orientation = Orientation::LEFTTOP);

    // Raster should be uint32[width * height]
    // Samples per pixel is 4
    bool WriteTIFF(std::string_view filepath,
                   uint16_t layerCount,
                   uint32_t width,
                   uint32_t height,
                   uint32_t* const* const rasters,
                   const Pencil* const pencils);
}
