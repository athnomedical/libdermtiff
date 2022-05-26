#include <assert.h>

#include <iostream>

#include "libdermtiff/dermtiff.hpp"

bool called = false;

std::string TypeToStr(ldt::msg::Type type) {
    switch (type) {
    case ldt::msg::Type::Info:
        return "Info";
    case ldt::msg::Type::Warning:
        return "Warning";
    case ldt::msg::Type::Error:
        return "Error";
    default:
        assert(false);
        return "Undefined";
    }
}

void callback(ldt::msg::Type type, const std::string &message) {
    std::cout << "[" << TypeToStr(type) << "] " << message << std::endl;
    called = true;
}

int main() {
    ldt::msg::RegisterCallback(callback);

    ldt::io::OpenTIFF("");
    assert(called);
    called = false;

    ldt::msg::RemoveCallback();

    ldt::io::OpenTIFF("");
    assert(!called);
    called = false;

    return 0;
}
