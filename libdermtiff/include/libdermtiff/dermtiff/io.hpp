#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "pencil.hpp"
#include "type.hpp"

namespace ldt {
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
    DermTIFF Open(const std::string& filepath);

    // Read the specific page
    // - Page 0		: Original image
    // - Page 1...	: Layers
    // Raster should be uint32[width * height]
    bool ReadPage(const std::string& filepath,
                  uint16_t page,
                  uint32_t* raster,
                  Pencil* pencil,
                  Orientation orientation = Orientation::LEFTTOP);

    // Read the original image of dermtiff
    // Raster should be uint32[width * height]
    bool ReadOriginalImage(const std::string& filepath,
                           uint32_t* raster,
                           Orientation orientation = Orientation::LEFTTOP);

    // Read the specific layer of dermtiff
    // Raster should be uint32[width * height]
    bool ReadLayer(const std::string& filepath,
                   uint16_t layerIndex,
                   uint32_t* raster,
                   Pencil* pencil,
                   Orientation orientation = Orientation::LEFTTOP);

    // Raster should be uint32[width * height]
    // Samples per pixel is 4
    bool WriteTIFF(const std::string& filepath,
                   uint16_t layerCount,
                   uint32_t width,
                   uint32_t height,
                   uint32_t* rasters[],
                   Pencil pencils[]);
}
