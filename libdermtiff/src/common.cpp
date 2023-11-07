#include "common.hpp"

#define STRINGFY2(s) #s
#define STRINGFY(s) STRINGFY2(s)

#define LIBDERMTIFF_VERSION                                                                                            \
    (LIBDERMTIFF_VERSION_MAJOR * 10000 + LIBDERMTIFF_VERSION_MINOR * 100 + LIBDERMTIFF_VERSION_PATCH)

namespace ldt {
    EXPORT const char* STDCALL GetVersion() {
        return STRINGFY(LIBDERMTIFF_VERSION_MAJOR) "." STRINGFY(LIBDERMTIFF_VERSION_MINOR) "." STRINGFY(
            LIBDERMTIFF_VERSION_PATCH) ".";
    }

    EXPORT const char* STDCALL GetVersionNum() {
        return STRINGFY(LIBDERMTIFF_VERSION);
    }
}
