#include "pencil.hpp"

#include <iostream>
#include <sstream>

#include "message_detail.hpp"

namespace ldt {
    namespace _internal {
        void ltrim(std::string& str, const std::string& trimChars) {
            if (!str.empty()) {
                const size_t pos = str.find_first_not_of(trimChars);
                str              = str.substr(pos, str.length() - pos);
            }
        }

        void rtrim(std::string& str, const std::string& trimChars) {
            if (!str.empty()) {
                const size_t pos = str.find_last_not_of(trimChars);
                str              = str.substr(0, pos + 1);
            }
        }

        void trim(std::string& str, const std::string& trimChars) {
            _internal::ltrim(str, trimChars);
            _internal::rtrim(str, trimChars);
        }

        void trimSpaces(std::string& str) {
            _internal::trim(str, " 　");  // Half-width space and full-width space
        }
    }

    std::optional<Pencil> Pencil::Parse(const std::string& str) {
        Pencil pencil;

        const auto index = str.find('/');
        if (index == std::string::npos) {
            msg::Output(msg::Type::Error, "Pencil::Parse", "No separator in string");
            return std::nullopt;
        }

        pencil.name = str.substr(0, index);
        _internal::trimSpaces(pencil.name);
        if (pencil.name.empty()) {
            msg::Output(msg::Type::Error, "Pencil::Parse", "Empty pencil name");
            return std::nullopt;
        }

        try {
            auto colorString = str.substr(index + 1, str.length() - index);
            _internal::trimSpaces(colorString);
            _internal::trim(colorString, "()");
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
                msg::Output(msg::Type::Error, "Pencil::Parse", "Invalid pencil color");
                return std::nullopt;
            }

            return pencil;
        } catch (const std::exception& e) {
            msg::Output(msg::Type::Error, "Pencil::Parse", std::string(e.what()));
            return std::nullopt;
        }
    }

    std::optional<std::string> Pencil::toString() const {
        if (name.empty()) {
            msg::Output(msg::Type::Error, "Pencil::toString", "Empty pencil name");
            return std::nullopt;
        }

        std::stringstream stream{""};
        stream << name << "/(" << +r << "," << +g << "," << +b << "," << +a << ")";
        return stream.str();
    }
}
