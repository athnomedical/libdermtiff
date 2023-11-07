#include "dermtiff/io.hpp"

#include <libtiff/tiffio.h>

#include <vector>

#include "message_detail.hpp"
#include "util.hpp"

namespace ldt::io {
    namespace _internal {
        bool TIFFSetDefaultFields(
            TIFF* const tiff, uint32_t width, uint32_t height, uint16_t page, uint16_t pageCount) {
            constexpr int16_t extraSamples[] = {EXTRASAMPLE_UNASSALPHA};
            const auto result                = TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width) == 1
                                && TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height) == 1
                                && TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_ADOBE_DEFLATE) == 1
                                && TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB) == 1
                                && TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG) == 1
                                && TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 4) == 1
                                && TIFFSetField(tiff, TIFFTAG_EXTRASAMPLES, 1, extraSamples) == 1
                                && TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8) == 1
                                && TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT) == 1
                                && TIFFSetField(tiff, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE) == 1
                                && TIFFSetField(tiff, TIFFTAG_PAGENUMBER, page, pageCount) == 1;

            if (!result) {
                msg::Output(msg::Type::Error, "io::TIFFSetDefaultFields", "Could not set some tiff tags");
            }

            return result;
        }

        bool WriteImage(TIFF* const tiff, uint32_t width, uint32_t height, const uint32_t* const raster) {
            for (uint32_t y = 0; y < height; y++) {
                const auto pos = y * width;
                // raster + pos is the pointer of the image[y][0]
                // Since libtiff interface does not support const, so use const_cast to remove const
                // Writing process only reads the value, there is no change
                if (TIFFWriteScanline(tiff, static_cast<void*>(const_cast<uint32_t*>(raster) + pos), y) != 1) {
                    msg::Output(msg::Type::Error, "io::WriteImage", "Could not write the image");
                    return false;
                }
            }
            return true;
        }

        bool ReadPageImpl(const std::shared_ptr<TIFF>& tiffPtr,
                          uint16_t page,
                          uint32_t* raster,
                          Pencil* pencil,
                          Orientation orientation) {
            TIFF* const tiff    = tiffPtr.get();
            const auto dermTiff = DermTIFF(tiff);

            if (TIFFSetDirectory(tiff, page) != 1) {
                msg::Output(msg::Type::Error, "io::ReadPage", "Could not set the directory");
                return false;
            }

            if (TIFFReadRGBAImageOriented(
                    tiff, dermTiff.width, dermTiff.height, raster, static_cast<int>(orientation), 0)
                != 1) {
                msg::Output(msg::Type::Error, "io::ReadPage", "Could not read the image");
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

        bool WriteTIFFImpl(const std::shared_ptr<TIFF>& tiffPtr,
                           uint16_t layerCount,
                           uint32_t width,
                           uint32_t height,
                           const uint32_t* const* const rasters,
                           const Pencil* const pencils) {
            const uint16_t pageCount = layerCount + 1;

            // check parameters
            if (width > DermTIFF::MaxWidth || height > DermTIFF::MaxHeight) {
                msg::Output(msg::Type::Error, "io::WriteTIFF", "Too large to write");
                return false;
            }
            for (uint16_t i = 0; i < layerCount; i++) {
                // layer has pencil with empty name
                if (!pencils[i].toString().has_value()) {
                    return false;
                }
            }

            TIFF* const tiff = tiffPtr.get();

            for (uint16_t page = 0; page < pageCount; page++) {
                TIFFCreateDirectory(tiff);

                if (TIFFSetDefaultFields(tiff, width, height, page, pageCount)) {
                    // layers
                    if (page != 0) {
                        const auto penStr = pencils[page - 1].toString();
                        if (!penStr.has_value() || TIFFSetField(tiff, TIFFTAG_PAGENAME, penStr.value().c_str()) != 1) {
                            msg::Output(msg::Type::Error, "io::WriteTIFF", "Could not set the pencil name");
                            return false;
                        }
                    }
                } else {
                    return false;
                }

                if (!WriteImage(tiff, width, height, &*(rasters[page]))) {
                    return false;
                }

                TIFFWriteDirectory(tiff);
            }

            return true;
        }
    }

    DermTIFF OpenTIFF(const char* filepath) {
        return DermTIFF(filepath);
    }

    bool ReadPage(const char* filepath, uint16_t page, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        if (const auto tiffPtr = util::SafeTIFFOpen(filepath, "r"); tiffPtr) {
            return _internal::ReadPageImpl(tiffPtr, page, &*raster, &*pencil, orientation);
        }

        return false;
    }

    bool ReadOriginalImage(const char* filepath, uint32_t* raster, Orientation orientation) {
        return ReadPage(filepath, 0, &*raster, nullptr, orientation);
    }

    bool ReadLayer(
        const char* filepath, uint16_t layerIndex, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        return ReadPage(filepath, layerIndex + 1, &*raster, &*pencil, orientation);
    }

    bool WriteTIFF(const char* filepath,
                   uint16_t layerCount,
                   uint32_t width,
                   uint32_t height,
                   const uint32_t* const* const rasters,
                   const Pencil* const pencils) {
        if (const auto tiffPtr = util::SafeTIFFOpen(filepath, "w"); tiffPtr) {
            return _internal::WriteTIFFImpl(tiffPtr, layerCount, width, height, &*rasters, &*pencils);
        }
        return false;
    }

#ifdef _WIN32
    DermTIFF OpenTIFFW(const wchar_t* filepath) {
        return DermTIFF(filepath);
    }

    bool ReadPageW(const wchar_t* filepath, uint16_t page, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        if (const auto tiffPtr = util::SafeTIFFOpenW(filepath, "r"); tiffPtr) {
            return _internal::ReadPageImpl(tiffPtr, page, &*raster, &*pencil, orientation);
        }

        return false;
    }

    bool ReadOriginalImageW(const wchar_t* filepath, uint32_t* raster, Orientation orientation) {
        return ReadPageW(filepath, 0, &*raster, nullptr, orientation);
    }

    bool ReadLayerW(
        const wchar_t* filepath, uint16_t layerIndex, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        return ReadPageW(filepath, layerIndex + 1, &*raster, &*pencil, orientation);
    }

    bool WriteTIFFW(const wchar_t* filepath,
                    uint16_t layerCount,
                    uint32_t width,
                    uint32_t height,
                    const uint32_t* const* const rasters,
                    const Pencil* const pencils) {
        if (const auto tiffPtr = util::SafeTIFFOpenW(filepath, "w"); tiffPtr) {
            return _internal::WriteTIFFImpl(tiffPtr, layerCount, width, height, &*rasters, &*pencils);
        }
        return false;
    }
#endif
}
