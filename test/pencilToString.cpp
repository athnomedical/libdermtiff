#include <assert.h>

#include "libdermtiff/dermtiff.hpp"

int main() {
    {
        ldt::Pencil pencil;
        pencil.name = "";
        assert(!pencil.toString().has_value());
    }

    {
        ldt::Pencil pencil;
        pencil.name = "pencil";
        pencil.r    = 255;
        pencil.g    = 255;
        pencil.b    = 255;
        pencil.a    = 255;
        assert(pencil.toString() == "pencil/(255,255,255,255)");
    }

    return 0;
}
