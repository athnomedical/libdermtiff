#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "libdermtiff/dermtiff.hpp"

TEST_CASE("Register callback and call", "[msg::RegisterCallback]") {
    bool called = false;
    ldt::msg::RegisterCallback([&called](auto, auto) { called = true; });
    ldt::io::OpenTIFF("");  // Open nothing to trigger an error
    REQUIRE(called);
}

TEST_CASE("Remove callback", "[msg::RegisterCallback]") {
    bool called = false;
    ldt::msg::RegisterCallback([&called](auto, auto) { called = true; });
    ldt::msg::RemoveCallback();
    ldt::io::OpenTIFF("");  // Open nothing to trigger an error
    REQUIRE_FALSE(called);
}

TEST_CASE("Overwrite callback", "[msg::RegisterCallback]") {
    bool called = false, called2 = false;
    ldt::msg::RegisterCallback([&called](auto, auto) { called = true; });
    ldt::msg::RegisterCallback([&called2](auto, auto) { called2 = true; });
    ldt::io::OpenTIFF("");  // Open nothing to trigger an error
    REQUIRE_FALSE(called);
    REQUIRE(called2);
}
