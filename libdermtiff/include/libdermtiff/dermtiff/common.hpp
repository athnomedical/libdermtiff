#pragma once

#define LIBDERMTIFF_VERSION_MAJOR 0
#define LIBDERMTIFF_VERSION_MINOR 4
#define LIBDERMTIFF_VERSION_PATCH 1

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#define STDCALL __stdcall
#else
#define EXPORT extern "C"
#define STDCALL
#endif

namespace ldt {
    EXPORT const char* STDCALL get_version();
}
