#include "derm_tiff.hpp"

#include <libtiff/tiffio.h>

#include <functional>
#include <memory>
#include <sstream>

#include "impl/message/message.hpp"
#include "impl/tiff_util/tiff_util.hpp"

#define ValidateDetail(errorMessage, process)                                                                          \
    {                                                                                                                  \
        bool valid = true;                                                                                             \
        process;                                                                                                       \
        isValid &= valid;                                                                                              \
        if (!valid) {                                                                                                  \
            msg::Print(msg::Type::Error, "DermTIFF::Validate", errorMessage);                                          \
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
                    valid &= dermTiff.width == tiff_util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
                    valid &= dermTiff.height == tiff_util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
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
            ValidateDetail(ss.str(), const auto compression = tiff_util::GetField<uint16_t>(tiff, TIFFTAG_COMPRESSION);
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

    DermTIFF OpenDermTiff(TIFF* const tiff) {
        DermTIFF dermTiff;
        dermTiff.pageCount       = _internal::SafeTIFFGetValue(tiff, TIFFNumberOfDirectories);
        dermTiff.layerCount      = dermTiff.pageCount - 1;
        dermTiff.width           = tiff_util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
        dermTiff.height          = tiff_util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
        dermTiff.isOpened        = tiff != nullptr;
        dermTiff.isValid         = _internal::Validate(dermTiff, tiff);
        dermTiff.bitsPerSample   = tiff_util::GetField<uint16_t>(tiff, TIFFTAG_BITSPERSAMPLE);
        dermTiff.compression     = tiff_util::GetField<uint16_t>(tiff, TIFFTAG_COMPRESSION);
        dermTiff.extraSamples    = _internal::GetExtraSamples(tiff);
        dermTiff.photometric     = tiff_util::GetField<uint16_t>(tiff, TIFFTAG_PHOTOMETRIC);
        dermTiff.planarConfig    = tiff_util::GetField<uint16_t>(tiff, TIFFTAG_PLANARCONFIG);
        dermTiff.samplesPerPixel = tiff_util::GetField<uint16_t>(tiff, TIFFTAG_SAMPLESPERPIXEL);
        dermTiff.subFileType     = tiff_util::GetField<uint32_t>(tiff, TIFFTAG_SUBFILETYPE);

        return dermTiff;
    }

    DermTIFF OpenDermTiff(std::string_view path) {
        return OpenDermTiff(tiff_util::OpenTiff(path, "r").get());
    }

#ifdef _WIN32
    DermTIFF OpenDermTiff(std::wstring_view path) {
        return OpenDermTiff(tiff_util::OpenTiffW(path, "r").get());
    }
#endif
}
