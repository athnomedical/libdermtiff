#include "dermtiff/type.hpp"

#include <libtiff/tiffio.h>

#include <functional>
#include <memory>
#include <sstream>

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
            ValidateDetail("All of image size of pages should be the same", {
                for (uint16_t i = 0; i < dermTiff.pageCount; i++) {
                    TIFFReadDirectory(tiff);
                    valid &= dermTiff.width == util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
                    valid &= dermTiff.height == util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
                }
                TIFFSetDirectory(tiff, 0);
            });

            // limitation of image width and height
            ValidateDetail(ss.str(), valid &= 0 < dermTiff.width && 0 < dermTiff.height;
                           valid &= dermTiff.width <= DermTIFF::MaxWidth && dermTiff.height <= DermTIFF::MaxHeight;
                           std::stringstream ss;
                           ss << "Image size " << dermTiff.width << "x" << dermTiff.height << " exceeds the limit "
                              << DermTIFF::MaxWidth << "x" << DermTIFF::MaxHeight;);

            // compression support
            ValidateDetail(ss.str(), const auto compression = util::GetField<uint16_t>(tiff, TIFFTAG_COMPRESSION);
                           valid &= TIFFIsCODECConfigured(compression) == 1;
                           std::stringstream ss;
                           ss << "The compression " << compression << " is not supported";);

            return isValid;
        }

        uint16_t SafeTIFFGetValue(TIFF* const tiff, const std::function<uint16_t(TIFF*)>& func) {
            if (tiff == nullptr) {
                return 0;
            } else {
                return func(tiff);
            }
        }

        uint16_t GetExtraSamples(TIFF* const tiff) {
            if (tiff == nullptr) {
                return 0;
            }

            uint16_t count    = 0;
            auto extraSamples = std::make_unique<uint16_t*>();

            if (TIFFGetField(tiff, TIFFTAG_EXTRASAMPLES, &count, extraSamples.get()) == 1) {
                uint16_t result = 0;
                for (size_t i = 0; i < count; i++) {
                    result += *extraSamples.get()[i] + 2;
                }
                return result;
            } else {
                return 0;
            }
        }
    }

    DermTIFF::DermTIFF(std::string_view path) : DermTIFF(util::SafeTIFFOpen(path, "r").get()) {}

    DermTIFF::DermTIFF(TIFF* const tiff) :
        pageCount(_internal::SafeTIFFGetValue(tiff, TIFFNumberOfDirectories)),
        layerCount(pageCount - 1),
        width(util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH)),
        height(util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH)),
        isOpened(tiff != nullptr),
        isValid(_internal::Validate(*this, tiff)),
        detail({util::GetField<uint16_t>(tiff, TIFFTAG_BITSPERSAMPLE),
                util::GetField<uint16_t>(tiff, TIFFTAG_COMPRESSION),
                _internal::GetExtraSamples(tiff),
                util::GetField<uint16_t>(tiff, TIFFTAG_PHOTOMETRIC),
                util::GetField<uint16_t>(tiff, TIFFTAG_PLANARCONFIG),
                util::GetField<uint16_t>(tiff, TIFFTAG_SAMPLESPERPIXEL),
                util::GetField<uint32_t>(tiff, TIFFTAG_SUBFILETYPE)}) {}

#ifdef _WIN32
    DermTIFF::DermTIFF(std::wstring_view path) : DermTIFF(util::SafeTIFFOpenW(path, "r").get()) {}
#endif
}
