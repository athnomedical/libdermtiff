#include "common.hpp"

namespace ldt {
    EXPORT const char* STDCALL GetVersion() {
        return NUM_TO_STR(LIBDERMTIFF_VERSION_MAJOR) "." NUM_TO_STR(LIBDERMTIFF_VERSION_MINOR) "." NUM_TO_STR(
            LIBDERMTIFF_VERSION_PATCH);
    }

    EXPORT const char* STDCALL GetVersionNum() {
        return NUM_TO_STR(LIBDERMTIFF_VERSION);
    }
}
