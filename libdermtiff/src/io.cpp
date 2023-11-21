#include "dermtiff/io.hpp"

#include <libtiff/tiffio.h>

#include <vector>

#include "impl/derm_tiff/derm_tiff.hpp"
#include "impl/message/message.hpp"
#include "impl/tiff_util/tiff_util.hpp"

namespace ldt::io {
    namespace internal {
        bool set_default_fields(TIFF* const tiff, uint32_t width, uint32_t height, uint16_t page, uint16_t page_count) {
            constexpr int16_t extra_samples[] = {EXTRASAMPLE_UNASSALPHA};
            const auto result                 = TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width) == 1
                                && TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height) == 1
                                && TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_ADOBE_DEFLATE) == 1
                                && TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB) == 1
                                && TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG) == 1
                                && TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 4) == 1
                                && TIFFSetField(tiff, TIFFTAG_EXTRASAMPLES, 1, extra_samples) == 1
                                && TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8) == 1
                                && TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT) == 1
                                && TIFFSetField(tiff, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE) == 1
                                && TIFFSetField(tiff, TIFFTAG_PAGENUMBER, page, page_count) == 1;

            if (!result) {
                msg::print(msg::Type::Error, "io::set_default_fields", "Could not set some tiff tags");
            }

            return result;
        }

        bool write_image(TIFF* const tiff, uint32_t width, uint32_t height, const uint32_t* const raster) {
            for (uint32_t y = 0; y < height; y++) {
                const auto pos = y * width;
                // raster + pos is the pointer of the image[y][0]
                // Since libtiff interface does not support const, so use const_cast to remove const
                // Writing process only reads the value, there is no change
                if (TIFFWriteScanline(tiff, static_cast<void*>(const_cast<uint32_t*>(raster) + pos), y) != 1) {
                    msg::print(msg::Type::Error, "io::write_image", "Could not write the image");
                    return false;
                }
            }
            return true;
        }

        bool read_page_impl(const std::shared_ptr<TIFF>& tiff_ptr,
                            uint16_t page,
                            uint32_t* raster,
                            Pencil* pencil,
                            Orientation orientation) {
            TIFF* const tiff     = tiff_ptr.get();
            const auto derm_tiff = open_derm_tiff(tiff);

            if (TIFFSetDirectory(tiff, page) != 1) {
                msg::print(msg::Type::Error, "io::ReadPage", "Could not set the directory");
                return false;
            }

            if (TIFFReadRGBAImageOriented(
                    tiff, derm_tiff.width, derm_tiff.height, raster, static_cast<int>(orientation), 0)
                != 1) {
                msg::print(msg::Type::Error, "io::ReadPage", "Could not read the image");
                return false;
            }

            if (pencil != nullptr) {
                // Read pencil
                if (const auto page_name = tiff_util::get_field_opt<char*>(tiff, TIFFTAG_PAGENAME);
                    page_name.has_value()) {
                    if (const auto result = Pencil::parse(page_name.value()); result.has_value()) {
                        *pencil = result.value();
                    }
                }
            }

            return true;
        }

        bool write_tiff_impl(const std::shared_ptr<TIFF>& tiff_ptr,
                             uint16_t layer_count,
                             uint32_t width,
                             uint32_t height,
                             const uint32_t* const* const rasters,
                             const Pencil* const pencils) {
            const uint16_t page_count = layer_count + 1;

            // check parameters
            if (width > DermTIFF::max_width || height > DermTIFF::max_height) {
                msg::print(msg::Type::Error, "io::write_tiff", "Too large to write");
                return false;
            }
            for (uint16_t i = 0; i < layer_count; i++) {
                // layer has pencil with empty name
                if (!pencils[i].to_string().has_value()) {
                    return false;
                }
            }

            TIFF* const tiff = tiff_ptr.get();

            for (uint16_t page = 0; page < page_count; page++) {
                TIFFCreateDirectory(tiff);

                if (set_default_fields(tiff, width, height, page, page_count)) {
                    // layers
                    if (page != 0) {
                        const auto pencil_str = pencils[page - 1].to_string();
                        if (!pencil_str.has_value()
                            || TIFFSetField(tiff, TIFFTAG_PAGENAME, pencil_str.value().c_str()) != 1) {
                            msg::print(msg::Type::Error, "io::write_tiff", "Could not set the pencil name");
                            return false;
                        }
                    }
                } else {
                    return false;
                }

                if (!write_image(tiff, width, height, &*(rasters[page]))) {
                    return false;
                }

                TIFFWriteDirectory(tiff);
            }

            return true;
        }
    }

    EXPORT DermTIFF STDCALL open_tiff(const char* filepath) {
        return open_derm_tiff(filepath);
    }

    EXPORT bool STDCALL
    read_page(const char* filepath, uint16_t page, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        if (const auto tiff = tiff_util::open_tiff(filepath, "r"); tiff) {
            return internal::read_page_impl(tiff, page, &*raster, &*pencil, orientation);
        }

        return false;
    }

    EXPORT bool STDCALL read_original_image(const char* filepath, uint32_t* raster, Orientation orientation) {
        return read_page(filepath, 0, &*raster, nullptr, orientation);
    }

    EXPORT bool STDCALL
    read_layer(const char* filepath, uint16_t layer_index, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        return read_page(filepath, layer_index + 1, &*raster, &*pencil, orientation);
    }

    EXPORT bool STDCALL write_tiff(const char* filepath,
                                   uint16_t layer_count,
                                   uint32_t width,
                                   uint32_t height,
                                   const uint32_t* const* const rasters,
                                   const Pencil* const pencils) {
        if (const auto tiff = tiff_util::open_tiff(filepath, "w"); tiff) {
            return internal::write_tiff_impl(tiff, layer_count, width, height, &*rasters, &*pencils);
        }
        return false;
    }

#ifdef _WIN32
    EXPORT DermTIFF STDCALL open_tiff_w(const wchar_t* filepath) {
        return open_derm_tiff(filepath);
    }

    EXPORT bool STDCALL
    read_page_w(const wchar_t* filepath, uint16_t page, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        if (const auto tiff = tiff_util::open_tiff_w(filepath, "r"); tiff) {
            return internal::read_page_impl(tiff, page, &*raster, &*pencil, orientation);
        }

        return false;
    }

    EXPORT bool STDCALL read_original_image_w(const wchar_t* filepath, uint32_t* raster, Orientation orientation) {
        return read_page_w(filepath, 0, &*raster, nullptr, orientation);
    }

    EXPORT bool STDCALL read_layer_w(
        const wchar_t* filepath, uint16_t layer_index, uint32_t* raster, Pencil* pencil, Orientation orientation) {
        return read_page_w(filepath, layer_index + 1, &*raster, &*pencil, orientation);
    }

    EXPORT bool STDCALL write_tiff_w(const wchar_t* filepath,
                                     uint16_t layer_count,
                                     uint32_t width,
                                     uint32_t height,
                                     const uint32_t* const* const rasters,
                                     const Pencil* const pencils) {
        if (const auto tiff = tiff_util::open_tiff_w(filepath, "w"); tiff) {
            return internal::write_tiff_impl(tiff, layer_count, width, height, &*rasters, &*pencils);
        }
        return false;
    }
#endif
}
