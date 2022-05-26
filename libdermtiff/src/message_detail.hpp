#pragma once

#include <string>

#include "../include/libdermtiff/dermtiff/message.hpp"

namespace ldt::msg {
    void Output(Type type, const std::string& funcName, const std::string& message);
}
