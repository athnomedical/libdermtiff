#include <assert.h>

#include "libdermtiff/dermtiff.hpp"

int main() {
    {
        ldt::Pencil pencil;
        pencil.setName("");
        assert(!pencil.toString().has_value());
    }

    {
        ldt::Pencil pencil;
        pencil.setName("pencil");
        pencil.r = 255;
        pencil.g = 255;
        pencil.b = 255;
        pencil.a = 255;
        assert(pencil.toString() == "pencil/(255,255,255,255)");
    }

    // Exceeds limit
    {
        const std::string limit  = std::string(ldt::Pencil::MaxNameLength, 'a');
        const std::string exceed = limit + "b";

        ldt::Pencil pencil;
        pencil.setName(exceed);
        assert(std::string(pencil.name) == limit);
    }

    return 0;
}
