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

    // try to open tiff file
    DermTIFF Open(const std::string& filepath);

    // raster should be uint32[width * height]
    bool ReadPage(const std::string& filepath,
                  uint16_t page,
                  uint32_t* raster,
                  Pencil* pencil,
                  Orientation orientation = Orientation::LEFTTOP);

    // raster should be uint32[width * height]
    // samples per pixel is 4
    bool WriteTIFF(const std::string& filepath,
                   uint16_t pageCount,
                   uint32_t width,
                   uint32_t height,
                   uint32_t* raster[],
                   Pencil pencil[]);
}
