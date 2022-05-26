#include "util.hpp"

#include <libtiff/tiffio.h>

#include "message_detail.hpp"

namespace ldt::util {
    namespace _internal {
        void SafeTIFFClose(TIFF* const tiff) noexcept {
            if (tiff != nullptr) {
                TIFFClose(tiff);
            }
        }

        template <typename T>
        T GetFieldUInt(TIFF* const tiff, uint32_t tag) noexcept {
            static_assert(std::is_unsigned<T>::value);
            if (tiff == nullptr) {
                return 0;
            } else {
                T value;
                TIFFGetField(tiff, tag, &value);
                return value;
            }
        }
    }

    template <>
    uint16_t GetField(TIFF* const tiff, uint32_t tag) noexcept {
        return _internal::GetFieldUInt<uint16_t>(tiff, tag);
    }

    template <>
    uint32_t GetField(TIFF* const tiff, uint32_t tag) noexcept {
        return _internal::GetFieldUInt<uint32_t>(tiff, tag);
    }

    std::shared_ptr<TIFF> SafeTIFFOpen(const std::string& path, const char* mode) noexcept {
        TIFF* const tiff = TIFFOpen(path.c_str(), mode);
        if (tiff == nullptr) {
            msg::Output(msg::Type::Error, "util::SafeTIFFOpen", "Could not open the tiff");
        }
        return std::shared_ptr<TIFF>(tiff, _internal::SafeTIFFClose);
    }
}
