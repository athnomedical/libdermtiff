#pragma once

#include <string>

#include "dermtiff/derm_tiff.hpp"

namespace ldt {
    DermTIFF open_derm_tiff(TIFF* const tiff);

    DermTIFF open_derm_tiff(std::string_view path);

#ifdef _WIN32
    DermTIFF open_derm_tiff(std::wstring_view path);
#endif
}
