#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "common.hpp"
#include "derm_tiff.hpp"
#include "pencil.hpp"

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
    EXPORT DermTIFF STDCALL OpenTIFF(const char* filepath);

    // Read the specific page
    // - Page 0		: Original image
    // - Page 1...	: Layers
    // Raster should be uint32[width * height]
    EXPORT bool STDCALL ReadPage(const char* filepath,
                                 uint16_t page,
                                 uint32_t* raster,
                                 Pencil* pencil,
                                 Orientation orientation = Orientation::LEFTTOP);

    // Read the original image of dermtiff
    // Raster should be uint32[width * height]
    EXPORT bool STDCALL ReadOriginalImage(const char* filepath,
                                          uint32_t* raster,
                                          Orientation orientation = Orientation::LEFTTOP);

    // Read the specific layer of dermtiff
    // Raster should be uint32[width * height]
    EXPORT bool STDCALL ReadLayer(const char* filepath,
                                  uint16_t layerIndex,
                                  uint32_t* raster,
                                  Pencil* pencil,
                                  Orientation orientation = Orientation::LEFTTOP);

    // Raster should be uint32[width * height]
    // Samples per pixel is 4
    EXPORT bool STDCALL WriteTIFF(const char* filepath,
                                  uint16_t layerCount,
                                  uint32_t width,
                                  uint32_t height,
                                  const uint32_t* const* const rasters,
                                  const Pencil* const pencils);

#ifdef _WIN32
    EXPORT DermTIFF STDCALL OpenTIFFW(const wchar_t* filepath);

    EXPORT bool STDCALL ReadPageW(const wchar_t* filepath,
                                  uint16_t page,
                                  uint32_t* raster,
                                  Pencil* pencil,
                                  Orientation orientation = Orientation::LEFTTOP);
    EXPORT bool STDCALL ReadOriginalImageW(const wchar_t* filepath,
                                           uint32_t* raster,
                                           Orientation orientation = Orientation::LEFTTOP);

    EXPORT bool STDCALL ReadLayerW(const wchar_t* filepath,
                                   uint16_t layerIndex,
                                   uint32_t* raster,
                                   Pencil* pencil,
                                   Orientation orientation = Orientation::LEFTTOP);

    EXPORT bool STDCALL WriteTIFFW(const wchar_t* filepath,
                                   uint16_t layerCount,
                                   uint32_t width,
                                   uint32_t height,
                                   const uint32_t* const* const rasters,
                                   const Pencil* const pencils);
#endif
}
