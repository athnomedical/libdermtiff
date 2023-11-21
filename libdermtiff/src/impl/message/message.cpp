#include "message.hpp"

#include <iostream>

namespace ldt::msg {
    namespace internal {
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

    void print(Type type, std::string_view funcName, std::string_view message) {
        // ex. libdermtiff[Error](FuncName) Could not read page
        std::cerr << "libdermtiff[" << internal::type_to_str(type) << "]"
                  << "(" << funcName << ") " << message << std::endl;
    }
}
