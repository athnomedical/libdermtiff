#include "tiff_util.hpp"

#include <libtiff/tiffio.h>

#include "impl/message/message.hpp"

namespace ldt::tiff_util {
    namespace internal {
        void close_tiff(TIFF* const tiff) noexcept {
            if (tiff != nullptr) {
                TIFFClose(tiff);
            }
        }

        template <typename T>
        T get_field_uint(TIFF* const tiff, uint32_t tag) noexcept {
            static_assert(std::is_unsigned<T>::value);
            if (tiff == nullptr) {
                return 0;
            } else {
                T value;
                TIFFGetField(tiff, tag, &value);
                return value;
            }
        }

        std::shared_ptr<TIFF> open_tiff(TIFF* const tiff) {
            if (tiff == nullptr) {
                msg::print(msg::Type::Error, "tiff_util::safe_tiff_open", "Could not open the tiff");
            }
            return std::shared_ptr<TIFF>(tiff, internal::close_tiff);
        }
    }

    template <>
    uint16_t get_field(TIFF* const tiff, uint32_t tag) noexcept {
        return internal::get_field_uint<uint16_t>(tiff, tag);
    }

    template <>
    uint32_t get_field(TIFF* const tiff, uint32_t tag) noexcept {
        return internal::get_field_uint<uint32_t>(tiff, tag);
    }

    std::shared_ptr<TIFF> open_tiff(std::string_view path, const char* mode) noexcept {
        return internal::open_tiff(TIFFOpen(std::string(path).c_str(), mode));
    }

#ifdef _WIN32
    std::shared_ptr<TIFF> open_tiff_w(std::wstring_view path, const char* mode) noexcept {
        return internal::open_tiff(TIFFOpenW(std::wstring(path).c_str(), mode));
    }
#endif
}
