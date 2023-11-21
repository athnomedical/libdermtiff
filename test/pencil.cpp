#define CATCH_CONFIG_MAIN

#include <sstream>

#include "catch2/catch.hpp"
#include "libdermtiff/dermtiff.hpp"

std::string create_pencil_string(const std::string& name, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    std::stringstream ss;
    ss << name << "/(" << +r << "," << +g << "," << +b << "," << +a << ")";
    return ss.str();
}

bool test_pencil_string_format(const ldt::Pencil& pencil) {
    return create_pencil_string(pencil.name, pencil.r, pencil.g, pencil.b, pencil.a) == pencil.to_string();
}

bool test_string_parse_to_pencil(std::string_view str, const ldt::Pencil& pencil) {
    return ldt::Pencil::parse(str) == pencil;
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

TEST_CASE("Pencil string is 'name/(r,g,b,a)'", "[Pencil::to_string]") {
    REQUIRE(test_pencil_string_format(ldt::Pencil{"pencil", 1, 2, 3, 4}));
    REQUIRE(test_pencil_string_format(ldt::Pencil{"pencil", 255, 255, 255, 255}));
    REQUIRE(test_pencil_string_format(ldt::Pencil{"pencil", 0, 10, 100, 255}));
}

TEST_CASE("parse string to Pencil", "[Pencil::parse]") {
    SECTION("Spaces in color") {
        REQUIRE(test_string_parse_to_pencil("pencil/( 1 , 2 , 3 , 4 )", ldt::Pencil{"pencil", 1, 2, 3, 4}));
    }

    SECTION("Spaces at edges of color") {
        REQUIRE(test_string_parse_to_pencil("pencil/ (1,2,3,4) ", ldt::Pencil{"pencil", 1, 2, 3, 4}));
    }

    SECTION("Spaces at edges of name") {
        REQUIRE(test_string_parse_to_pencil(" pencil /(1,2,3,4)", ldt::Pencil{"pencil", 1, 2, 3, 4}));
    }
}

TEST_CASE("Unparsable string to Pencil", "[Pencil::parse]") {
    SECTION("Empty string") {
        REQUIRE_FALSE(ldt::Pencil::parse("").has_value());
    }

    SECTION("Empty name and color") {
        REQUIRE_FALSE(ldt::Pencil::parse("/").has_value());

        REQUIRE_FALSE(ldt::Pencil::parse("/()").has_value());

        REQUIRE_FALSE(ldt::Pencil::parse("/(,,,)").has_value());
    }

    SECTION("Double slash") {
        REQUIRE_FALSE(ldt::Pencil::parse("//(0,0,0,0)").has_value());

        REQUIRE_FALSE(ldt::Pencil::parse("pencil//(0,0,0,0)").has_value());
    }

    SECTION("Color is not enough") {
        REQUIRE_FALSE(ldt::Pencil::parse("pencil/()").has_value());

        REQUIRE_FALSE(ldt::Pencil::parse("pencil/(0,0,0)").has_value());
    }
}
