#pragma once

#define LIBDERMTIFF_VERSION_MAJOR 0
#define LIBDERMTIFF_VERSION_MINOR 4
#define LIBDERMTIFF_VERSION_PATCH 1
#define LIBDERMTIFF_VERSION                                                                                            \
    (LIBDERMTIFF_VERSION_MAJOR * 10000 + LIBDERMTIFF_VERSION_MINOR * 100 + LIBDERMTIFF_VERSION_PATCH)

#define NUM_TO_STR(N) #N

#define EXPORT extern "C" __declspec(dllexport)
#define STDCALL __stdcall

namespace ldt {
    EXPORT const char* STDCALL GetVersion();

    EXPORT const char* STDCALL GetVersionNum();
}
