#include "pencil.hpp"

#include <iostream>
#include <sstream>

namespace ldt {
    std::optional<Pencil> Pencil::Parse(const std::string& str) {
        Pencil pencil;

        const auto index = str.find('/');
        if (index == std::string::npos) {
            return std::nullopt;
        }

        pencil.name = str.substr(0, index);
        if (pencil.name.empty()) {
            return std::nullopt;
        }

        try {
            const auto colorString = str.substr(index + 2, str.length() - index - 1);  // ignore '(' and ')'
            std::stringstream stream{colorString};
            std::string buf;
            size_t count = 0;
            while (std::getline(stream, buf, ',')) {
                const uint8_t val = static_cast<uint8_t>(std::stoi(buf));
                if (count == 0) {
                    pencil.r = val;
                } else if (count == 1) {
                    pencil.g = val;
                } else if (count == 2) {
                    pencil.b = val;
                } else if (count == 3) {
                    // Pencil alpha rate must be 255
                    pencil.a = 255;
                }
                count++;
            }

            if (count != 4) {
                return std::nullopt;
            }

            return pencil;
        } catch (std::exception e) {
            std::cerr << e.what() << std::endl;
            return std::nullopt;
        }
    }

    std::optional<std::string> Pencil::toString() const {
        if (name.empty()) {
            return std::nullopt;
        }

        std::stringstream stream{""};
        stream << name << "/(" << +r << "," << +g << "," << +b << "," << +a << ")";
        return stream.str();
    }
}
