#include "tiff_reader.hpp"

#include <libtiff/tiffio.h>

#include "impl/message/message.hpp"

namespace ldt::tiff_reader {
    namespace _internal {
        void CloseTiff(TIFF* const tiff) noexcept {
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

        std::shared_ptr<TIFF> OpenTiff(TIFF* const tiff) {
            if (tiff == nullptr) {
                msg::Print(msg::Type::Error, "tiff_reader::SafeTIFFOpen", "Could not open the tiff");
            }
            return std::shared_ptr<TIFF>(tiff, _internal::CloseTiff);
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

    std::shared_ptr<TIFF> OpenTiff(std::string_view path, const char* mode) noexcept {
        return _internal::OpenTiff(TIFFOpen(std::string(path).c_str(), mode));
    }

#ifdef _WIN32
    std::shared_ptr<TIFF> OpenTiffW(std::wstring_view path, const char* mode) noexcept {
        return _internal::OpenTiff(TIFFOpenW(std::wstring(path).c_str(), mode));
    }
#endif
}
