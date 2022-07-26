#include "message.hpp"

#include <iostream>

#include "message_detail.hpp"

namespace ldt::msg {
    namespace _internal {
        std::string TypeToStr(Type type) {
            switch (type) {
            case Type::Info:
                return "Info";
            case Type::Warning:
                return "Warning";
            case Type::Error:
                return "Error";
            default:
                return "Unknown";
            }
        }
    }

    bool stdErrWhenNoCallback = true;
    std::function<void(Type, std::string_view)> callback;

    void SetStdErrWhenNoCallback(bool enable) {
        stdErrWhenNoCallback = enable;
    }

    void Output(Type type, std::string_view funcName, std::string_view message) {
        if (callback) {
            callback(type, message);
        } else if (stdErrWhenNoCallback) {
            // ex. libdermtiff[Error](FuncName) Could not read page
            std::cerr << "libdermtiff[" << _internal::TypeToStr(type) << "]"
                      << "(" << funcName << ") " << message << std::endl;
        }
    }

    void RegisterCallback(const std::function<void(Type, std::string_view)>& _callback) {
        callback = _callback;
    }

    void RemoveCallback() {
        callback = std::function<void(Type, std::string_view)>();
    }
}
