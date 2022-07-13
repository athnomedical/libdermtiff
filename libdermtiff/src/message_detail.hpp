#pragma once

#include <string>

#include "../include/libdermtiff/dermtiff/message.hpp"

namespace ldt::msg {
    void Output(Type type, std::string_view funcName, std::string_view message);
}
