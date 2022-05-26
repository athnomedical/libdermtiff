#include "type.hpp"

#include <libtiff/tiffio.h>

#include <functional>

#include "message_detail.hpp"
#include "util.hpp"

#define ValidateDetail(errorMessage, process)                                                                          \
    {                                                                                                                  \
        bool valid = true;                                                                                             \
        process;                                                                                                       \
        isValid &= valid;                                                                                              \
        if (!valid) {                                                                                                  \
            msg::Output(msg::Type::Error, "DermTIFF::Validate", errorMessage);                                         \
        }                                                                                                              \
    }

namespace ldt {
    namespace _internal {
        bool Validate(const DermTIFF& dermTiff, TIFF* const tiff) {
            if (tiff == nullptr || dermTiff.pageCount == 0) {
                return false;
            }

            bool isValid = true;

            // image size is the same on all pages
            ValidateDetail("Size of images should be the same", {
                for (uint16_t i = 0; i < dermTiff.pageCount; i++) {
                    TIFFReadDirectory(tiff);
                    valid &= dermTiff.width == util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
                    valid &= dermTiff.height == util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
                }
                TIFFSetDirectory(tiff, 0);
            });

            // limitation of image width and height
            ValidateDetail("Image size exceeds the limit", {
                valid &= 0 < dermTiff.width && 0 < dermTiff.height;
                valid &= dermTiff.width <= DermTIFF::MaxWidth && dermTiff.height <= DermTIFF::MaxHeight;
            });

            // compression support
            ValidateDetail("The compression is not supported", {
                const auto compression = util::GetField<uint16_t>(tiff, TIFFTAG_COMPRESSION);
                valid &= TIFFIsCODECConfigured(compression) == 1;
            });

            return isValid;
        }

        uint16_t SafeTIFFGetValue(TIFF* const tiff, const std::function<uint16_t(TIFF*)>& func) {
            if (tiff == nullptr) {
                return 0;
            } else {
                return func(tiff);
            }
        }
    }

    DermTIFF::DermTIFF(const std::string& path) : DermTIFF(util::SafeTIFFOpen(path, "r").get()) {}

    DermTIFF::DermTIFF(TIFF* const tiff) :
        pageCount(_internal::SafeTIFFGetValue(tiff, TIFFNumberOfDirectories)),
        layerCount(pageCount - 1),
        width(util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH)),
        height(util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH)),
        isValid(_internal::Validate(*this, tiff)) {}
}
