#pragma once

#include <string>

#include "dermtiff/derm_tiff.hpp"

namespace ldt {
    DermTIFF OpenDermTiff(TIFF* const tiff);

    DermTIFF OpenDermTiff(std::string_view path);

#ifdef _WIN32
    DermTIFF OpenDermTiff(std::wstring_view path);
#endif
}
