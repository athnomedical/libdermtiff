#include <assert.h>

#include <sstream>

#include "libdermtiff/dermtiff.hpp"

bool testPencilStr(const std::string& str, const std::string& name, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    const auto result = ldt::Pencil::Parse(str);
    const auto parsed = ldt::Pencil{name, r, g, b, a};
    return result.has_value() && result.value() == parsed;
}

bool testPencil(const std::string& name, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    std::stringstream ss;
    ss << name << "/(" << +r << "," << +g << "," << +b << "," << +a << ")";
    return testPencilStr(ss.str(), name, r, g, b, a);
}

int main() {
    assert(testPencil("pencil", 255, 255, 255, 255));

    assert(testPencil("pencil", 0, 10, 100, 255));

    assert(testPencilStr("pencil/( 0 , 0 , 0 , 0 )", "pencil", 0, 0, 0, 0));

    assert(testPencilStr("pencil/ (0,0,0,0) ", "pencil", 0, 0, 0, 0));

    assert(testPencilStr(" pencil /(0,0,0,0)", "pencil", 0, 0, 0, 0));

    assert(!ldt::Pencil::Parse("").has_value());

    assert(!ldt::Pencil::Parse("/").has_value());

    assert(!ldt::Pencil::Parse("/()").has_value());

    assert(!ldt::Pencil::Parse("/(,,,)").has_value());

    assert(!ldt::Pencil::Parse("//(0,0,0,0)").has_value());

    assert(!ldt::Pencil::Parse("pencil/()").has_value());

    assert(!ldt::Pencil::Parse("pencil//(0,0,0,0)").has_value());

    assert(!ldt::Pencil::Parse("pencil/(0,0,0)").has_value());

    return 0;
}
