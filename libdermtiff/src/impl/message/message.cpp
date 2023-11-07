#include "message.hpp"

#include <iostream>

namespace ldt::msg {
    namespace _internal {
        std::string type_to_str(Type type) {
            switch (type) {
            case Type::Info:
                return "info";
            case Type::Warning:
                return "warning";
            case Type::Error:
                return "error";
            default:
                return "unknown";
            }
        }
    }

    void Print(Type type, std::string_view funcName, std::string_view message) {
        // ex. libdermtiff[Error](FuncName) Could not read page
        std::cerr << "libdermtiff[" << _internal::type_to_str(type) << "]"
                  << "(" << funcName << ") " << message << std::endl;
    }
}
