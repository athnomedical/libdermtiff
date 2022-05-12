#include <assert.h>

#include "libdermtiff/dermtiff.hpp"

int main() {
    {
        const auto result = ldt::Pencil::Parse("pencil/(255,255,255,255)");
        assert(result.has_value());

        const ldt::Pencil pencil = result.value();
        assert(pencil.name == "pencil");
        assert(pencil.r == 255);
        assert(pencil.g == 255);
        assert(pencil.b == 255);
        assert(pencil.a == 255);
    }

    {
        const auto result = ldt::Pencil::Parse("pencil/(0,10,100,255)");
        assert(result.has_value());

        const ldt::Pencil pencil = result.value();
        assert(pencil.name == "pencil");
        assert(pencil.r == 0);
        assert(pencil.g == 10);
        assert(pencil.b == 100);
        assert(pencil.a == 255);
    }

    {
        const auto result = ldt::Pencil::Parse("");
        assert(!result.has_value());
    }

    {
        const auto result = ldt::Pencil::Parse("/");
        assert(!result.has_value());
    }

    {
        const auto result = ldt::Pencil::Parse("/()");
        assert(!result.has_value());
    }

    {
        const auto result = ldt::Pencil::Parse("/(,,,)");
        assert(!result.has_value());
    }

    {
        const auto result = ldt::Pencil::Parse("//(0,0,0,0)");
        assert(!result.has_value());
    }

    {
        const auto result = ldt::Pencil::Parse("pencil/(0,0,0)");
        assert(!result.has_value());
    }

    return 0;
}
