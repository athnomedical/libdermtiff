#pragma once

#define LIBDERMTIFF_VERSION_MAJOR 0
#define LIBDERMTIFF_VERSION_MINOR 4
#define LIBDERMTIFF_VERSION_PATCH 1
#define LIBDERMTIFF_VERSION                                                                                            \
    (LIBDERMTIFF_VERSION_MAJOR * 10000 + LIBDERMTIFF_VERSION_MINOR * 100 + LIBDERMTIFF_VERSION_PATCH)

#define NUM_TO_STR(N) #N

#define EXPORT extern "C" __declspec(dllexport)
#define STDCALL __stdcall

EXPORT const char* STDCALL GetVersion() {
    return NUM_TO_STR(LIBDERMTIFF_VERSION_MAJOR) "." NUM_TO_STR(LIBDERMTIFF_VERSION_MINOR) "." NUM_TO_STR(
        LIBDERMTIFF_VERSION_PATCH);
}

EXPORT const char* STDCALL GetVersionNum() {
    return NUM_TO_STR(LIBDERMTIFF_VERSION);
}
