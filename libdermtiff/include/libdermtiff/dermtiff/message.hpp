#pragma once

#include <functional>
#include <string>

namespace ldt::msg {
    enum class Type { Info = 0, Warning, Error };

    void SetStdErrWhenNoCallback(bool enable);

    void RegisterCallback(const std::function<void(Type, std::string_view)>& callback);

    void RemoveCallback();
}
