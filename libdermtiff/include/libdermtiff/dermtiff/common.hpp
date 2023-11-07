#pragma once

#define LIBDERMTIFF_VERSION_MAJOR 0
#define LIBDERMTIFF_VERSION_MINOR 4
#define LIBDERMTIFF_VERSION_PATCH 1

#define EXPORT extern "C" __declspec(dllexport)
#define STDCALL __stdcall

namespace ldt {
    EXPORT const char* STDCALL GetVersion();
}
