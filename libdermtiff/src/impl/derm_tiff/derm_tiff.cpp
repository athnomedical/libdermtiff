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
        is_valid &= valid;                                                                                             \
        if (!valid) {                                                                                                  \
            msg::print(msg::Type::Error, "DermTIFF::validate_tiff", errorMessage);                                     \
        }                                                                                                              \
    }

namespace ldt {
    namespace internal {
        bool validate_tiff(const DermTIFF& derm_tiff, TIFF* const tiff) {
            if (tiff == nullptr || derm_tiff.page_count == 0) {
                return false;
            }

            bool is_valid = true;

            // image size is the same on all pages
            ValidateDetail("All of image size of pages should be the same", {
                for (uint16_t i = 0; i < derm_tiff.page_count; i++) {
                    TIFFReadDirectory(tiff);
                    valid &= derm_tiff.width == tiff_util::get_field<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
                    valid &= derm_tiff.height == tiff_util::get_field<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
                }
                TIFFSetDirectory(tiff, 0);
            });

            // limitation of image width and height
            ValidateDetail(ss.str(), valid &= 0 < derm_tiff.width && 0 < derm_tiff.height;
                           valid &= derm_tiff.width <= DermTIFF::max_width && derm_tiff.height <= DermTIFF::max_height;
                           std::stringstream ss;
                           ss << "Image size " << derm_tiff.width << "x" << derm_tiff.height << " exceeds the limit "
                              << DermTIFF::max_width << "x" << DermTIFF::max_height;);

            // compression support
            ValidateDetail(ss.str(), const auto compression = tiff_util::get_field<uint16_t>(tiff, TIFFTAG_COMPRESSION);
                           valid &= TIFFIsCODECConfigured(compression) == 1;
                           std::stringstream ss;
                           ss << "The compression " << compression << " is not supported";);

            return is_valid;
        }

        uint16_t safe_tiff_get_value(TIFF* const tiff, const std::function<uint16_t(TIFF*)>& func) {
            if (tiff == nullptr) {
                return 0;
            } else {
                return func(tiff);
            }
        }

        uint16_t get_extra_samples(TIFF* const tiff) {
            if (tiff == nullptr) {
                return 0;
            }

            uint16_t count          = 0;
            const auto extraSamples = std::make_unique<uint16_t*>();

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

    DermTIFF open_derm_tiff(TIFF* const tiff) {
        DermTIFF derm_tiff;
        derm_tiff.page_count        = internal::safe_tiff_get_value(tiff, TIFFNumberOfDirectories);
        derm_tiff.layer_count       = derm_tiff.page_count - 1;
        derm_tiff.width             = tiff_util::get_field<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
        derm_tiff.height            = tiff_util::get_field<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
        derm_tiff.bits_per_sample   = tiff_util::get_field<uint16_t>(tiff, TIFFTAG_BITSPERSAMPLE);
        derm_tiff.compression       = tiff_util::get_field<uint16_t>(tiff, TIFFTAG_COMPRESSION);
        derm_tiff.extra_samples     = internal::get_extra_samples(tiff);
        derm_tiff.photometric       = tiff_util::get_field<uint16_t>(tiff, TIFFTAG_PHOTOMETRIC);
        derm_tiff.planar_config     = tiff_util::get_field<uint16_t>(tiff, TIFFTAG_PLANARCONFIG);
        derm_tiff.samples_per_pixel = tiff_util::get_field<uint16_t>(tiff, TIFFTAG_SAMPLESPERPIXEL);
        derm_tiff.sub_file_type     = tiff_util::get_field<uint32_t>(tiff, TIFFTAG_SUBFILETYPE);
        derm_tiff.is_valid          = internal::validate_tiff(derm_tiff, tiff);

        return derm_tiff;
    }

    DermTIFF open_derm_tiff(std::string_view path) {
        return open_derm_tiff(tiff_util::open_tiff(path, "r").get());
    }

#ifdef _WIN32
    DermTIFF open_derm_tiff(std::wstring_view path) {
        return open_derm_tiff(tiff_util::open_tiff_w(path, "r").get());
    }
#endif
}
