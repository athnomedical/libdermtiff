#define CATCH_CONFIG_MAIN

#include <sstream>

#include "catch2/catch.hpp"
#include "libdermtiff/dermtiff.hpp"

std::string CreatePencilString(const std::string& name, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    std::stringstream ss;
    ss << name << "/(" << +r << "," << +g << "," << +b << "," << +a << ")";
    return ss.str();
}

bool TestPencilStringFormat(const ldt::Pencil& pencil) {
    return CreatePencilString(pencil.name, pencil.r, pencil.g, pencil.b, pencil.a) == pencil.toString();
}

bool TestStringParseToPencil(std::string_view str, const ldt::Pencil& pencil) {
    return ldt::Pencil::Parse(str) == pencil;
}

TEST_CASE("Max pencil name length is 64", "[Pencil]") {
    const std::string validName   = std::string(64, 'a');  // aaa...aaa
    const std::string invalidName = validName + 'b';       // aaa...aaab

    SECTION("Able to set 64 chars") {
        const auto pencil = ldt::Pencil{validName, 255, 255, 255, 255};
        REQUIRE(validName == pencil.name);
    }

    SECTION("Unable to set 64 chars") {
        const auto pencil = ldt::Pencil{invalidName, 255, 255, 255, 255};
        REQUIRE_FALSE(invalidName == pencil.name);

        SECTION("Name is trimmed") {
            REQUIRE(validName == pencil.name);
        }
    }
}

TEST_CASE("Pencil string is 'name/(r,g,b,a)'", "[Pencil::toString]") {
    REQUIRE(TestPencilStringFormat(ldt::Pencil{"pencil", 1, 2, 3, 4}));
    REQUIRE(TestPencilStringFormat(ldt::Pencil{"pencil", 255, 255, 255, 255}));
    REQUIRE(TestPencilStringFormat(ldt::Pencil{"pencil", 0, 10, 100, 255}));
}

TEST_CASE("Parse string to Pencil", "[Pencil::Parse]") {
    SECTION("Spaces in color") {
        REQUIRE(TestStringParseToPencil("pencil/( 1 , 2 , 3 , 4 )", ldt::Pencil{"pencil", 1, 2, 3, 4}));
    }

    SECTION("Spaces at edges of color") {
        REQUIRE(TestStringParseToPencil("pencil/ (1,2,3,4) ", ldt::Pencil{"pencil", 1, 2, 3, 4}));
    }

    SECTION("Spaces at edges of name") {
        REQUIRE(TestStringParseToPencil(" pencil /(1,2,3,4)", ldt::Pencil{"pencil", 1, 2, 3, 4}));
    }
}

TEST_CASE("Unparsable string to Pencil", "[Pencil::Parse]") {
    SECTION("Empty string") {
        REQUIRE_FALSE(ldt::Pencil::Parse("").has_value());
    }

    SECTION("Empty name and color") {
        REQUIRE_FALSE(ldt::Pencil::Parse("/").has_value());

        REQUIRE_FALSE(ldt::Pencil::Parse("/()").has_value());

        REQUIRE_FALSE(ldt::Pencil::Parse("/(,,,)").has_value());
    }

    SECTION("Double slash") {
        REQUIRE_FALSE(ldt::Pencil::Parse("//(0,0,0,0)").has_value());

        REQUIRE_FALSE(ldt::Pencil::Parse("pencil//(0,0,0,0)").has_value());
    }

    SECTION("Color is not enough") {
        REQUIRE_FALSE(ldt::Pencil::Parse("pencil/()").has_value());

        REQUIRE_FALSE(ldt::Pencil::Parse("pencil/(0,0,0)").has_value());
    }
}
