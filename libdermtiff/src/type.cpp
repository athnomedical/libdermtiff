#include "type.hpp"

#include <libtiff/tiffio.h>

#include <functional>

#include "util.hpp"

namespace ldt {
    namespace _internal {
        bool Validate(const DermTIFF& dermTiff, TIFF* const tiff) {
            if (tiff == nullptr || dermTiff.pageCount == 0) {
                return false;
            }

            bool isValid = true;

            // image size is the same on all pages
            for (uint16_t i = 0; i < dermTiff.pageCount; i++) {
                TIFFReadDirectory(tiff);
                isValid &= dermTiff.width == util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH);
                isValid &= dermTiff.height == util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH);
            }
            TIFFSetDirectory(tiff, 0);

            // limitation of image width and height
            isValid &= 0 < dermTiff.width && 0 < dermTiff.height;
            isValid &= dermTiff.width <= DermTIFF::MaxWidth && dermTiff.height <= DermTIFF::MaxHeight;

            // compression support
            {
                const auto compression = util::GetField<uint16_t>(tiff, TIFFTAG_COMPRESSION);
                isValid &= TIFFIsCODECConfigured(compression) == 1;
            }

            return isValid;
        }

        template <typename T>
        T SafeTIFFGetValue(TIFF* const tiff, const std::function<T(TIFF*)>& func) {
            if (tiff == nullptr) {
                return T();
            } else {
                return func(tiff);
            }
        }
    }

    DermTIFF::DermTIFF(const std::string& path) : DermTIFF(util::SafeTIFFOpen(path, "r").get()) {}

    DermTIFF::DermTIFF(TIFF* const tiff) :
        pageCount(_internal::SafeTIFFGetValue<uint16_t>(tiff, TIFFNumberOfDirectories)),
        layerCount(pageCount - 1),
        width(util::GetField<uint32_t>(tiff, TIFFTAG_IMAGEWIDTH)),
        height(util::GetField<uint32_t>(tiff, TIFFTAG_IMAGELENGTH)),
        photoMetric(util::GetField<uint16_t>(tiff, TIFFTAG_PHOTOMETRIC)),
        samplesPerPixel(util::GetField<uint16_t>(tiff, TIFFTAG_SAMPLESPERPIXEL)),
        scanlineSize(_internal::SafeTIFFGetValue<int64_t>(tiff, TIFFScanlineSize)),
        isValid(_internal::Validate(*this, tiff)) {}
}
