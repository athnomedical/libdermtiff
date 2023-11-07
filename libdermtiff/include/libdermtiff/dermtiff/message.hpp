#pragma once

#include <functional>
#include <string>

#define EXPORT extern "C" __declspec(dllexport)
#define STDCALL __stdcall

namespace ldt::msg {
    enum class Type { Info = 0, Warning, Error };

    EXPORT void STDCALL SetStdErrWhenNoCallback(bool enable);

    EXPORT void STDCALL RegisterCallback(const std::function<void(Type, std::string_view)>& callback);

    EXPORT void STDCALL RemoveCallback();
}
