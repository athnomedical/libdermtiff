#include "common.hpp"

#define STRINGFY2(s) #s
#define STRINGFY(s) STRINGFY2(s)

namespace ldt {
    EXPORT const char* STDCALL GetVersion() {
        return STRINGFY(LIBDERMTIFF_VERSION_MAJOR) "." STRINGFY(LIBDERMTIFF_VERSION_MINOR) "." STRINGFY(
            LIBDERMTIFF_VERSION_PATCH);
    }

}
