#pragma once

#include <string>

#include "message.hpp"

namespace ldt::msg {
    void Output(Type type, std::string_view funcName, std::string_view message);
}
