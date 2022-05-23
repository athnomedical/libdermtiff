#include "io.hpp"

#include <libtiff/tiffio.h>

#include <vector>

#include "util.hpp"

namespace ldt::io {
    namespace _internal {
        namespace writer {
            bool TIFFSetDefaultFields(
                TIFF* const tiff, uint32_t width, uint32_t height, uint16_t page, uint16_t pageCount) {
                constexpr int16_t extraSamples[] = {EXTRASAMPLE_UNASSALPHA};
                return TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width) == 1
                       && TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height) == 1
                       && TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW) == 1
                       && TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB) == 1
                       && TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG) == 1
                       && TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 4) == 1
                       && TIFFSetField(tiff, TIFFTAG_EXTRASAMPLES, 1, extraSamples) == 1
                       && TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8) == 1
                       && TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT) == 1
                       && TIFFSetField(tiff, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE) == 1
                       && TIFFSetField(tiff, TIFFTAG_PAGENUMBER, page, pageCount) == 1;
            }

            void WriteImage(TIFF* const tiff, uint32_t width, uint32_t height, uint32_t* raster) {
                for (uint32_t y = 0; y < height; y++) {
                    const auto pos = y * width;
                    TIFFWriteScanline(tiff, raster + pos, y);  // raster + pos is the pointer of the image[y][0]
                }
            }
        }
    }

    DermTIFF OpenTIFF(const std::string& filepath) {
        return DermTIFF(filepath);
    }

    bool ReadPage(
        const std::string& filepath, uint16_t page, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        if (const auto tiffPtr = util::SafeTIFFOpen(filepath, "r"); tiffPtr) {
            TIFF* const tiff    = tiffPtr.get();
            const auto dermTiff = DermTIFF(tiff);

            if (TIFFSetDirectory(tiff, page) != 1) {
                // could not set directory
                return false;
            }

            if (TIFFReadRGBAImageOriented(
                    tiff, dermTiff.width, dermTiff.height, raster, static_cast<int>(orientation), 0)
                != 1) {
                return false;
            }

            if (pencil != nullptr) {
                // Read pencil
                if (const auto pagename = util::GetFieldOpt<char*>(tiff, TIFFTAG_PAGENAME); pagename.has_value()) {
                    if (const auto result = Pencil::Parse(pagename.value()); result.has_value()) {
                        *pencil = result.value();
                    }
                }
            }

            return true;
        }

        return false;
    }

    bool ReadOriginalImage(const std::string& filepath, uint32_t* raster, Orientation orientation) {
        return ReadPage(filepath, 0, &*raster, nullptr, orientation);
    }

    bool ReadLayer(
        const std::string& filepath, uint16_t layerIndex, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        return ReadPage(filepath, layerIndex + 1, &*raster, &*pencil, orientation);
    }

    bool WriteTIFF(const std::string& filepath,
                   uint16_t layerCount,
                   uint32_t width,
                   uint32_t height,
                   uint32_t* rasters[],
                   Pencil pencils[]) {
        const uint16_t pageCount = layerCount + 1;

        // check parameters
        if (width > DermTIFF::MaxWidth || height > DermTIFF::MaxHeight) {
            return false;
        }
        for (uint16_t i = 0; i < layerCount; i++) {
            // layer has pencil with empty name
            if (!pencils[i].toString().has_value()) {
                return false;
            }
        }

        if (const auto tiffPtr = util::SafeTIFFOpen(filepath, "w"); tiffPtr) {
            TIFF* const tiff = tiffPtr.get();

            for (uint16_t page = 0; page < pageCount; page++) {
                TIFFCreateDirectory(tiff);

                // original image
                if (page == 0) {
                    if (!_internal::writer::TIFFSetDefaultFields(tiff, width, height, page, pageCount)) {
                        return false;
                    }
                }
                // layers
                else {
                    const auto penStr = pencils[page - 1].toString();
                    if (!penStr.has_value()
                        || !_internal::writer::TIFFSetDefaultFields(tiff, width, height, page, pageCount)
                        || TIFFSetField(tiff, TIFFTAG_PAGENAME, penStr.value().c_str()) != 1) {
                        return false;
                    }
                }

                _internal::writer::WriteImage(tiff, width, height, &*(rasters[page]));

                TIFFWriteDirectory(tiff);
            }

            return true;
        }

        return false;
    }
}
