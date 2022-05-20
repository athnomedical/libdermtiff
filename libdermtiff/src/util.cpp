#include "util.hpp"

#include <libtiff/tiffio.h>

namespace ldt::util {
    namespace _internal {
        void SafeTIFFClose(TIFF* const tiff) noexcept {
            if (tiff != nullptr) {
                TIFFClose(tiff);
            }
        }
    }

    std::shared_ptr<TIFF> SafeTIFFOpen(const std::string& path, const char* mode) noexcept {
        return std::shared_ptr<TIFF>(TIFFOpen(path.c_str(), mode), _internal::SafeTIFFClose);
    }
}
