#include "dermtiff/pencil.hpp"

#include <iostream>
#include <sstream>

#include "impl/message/message.hpp"

#if _WIN32
#define STRCPY strcpy_s
#else
#define STRCPY strcpy
#endif

namespace ldt {
    namespace internal {
        void ltrim(std::string& target, const std::string& str) {
            if (!target.empty()) {
                const size_t pos = target.find_first_not_of(str);
                target           = target.substr(pos, target.length() - pos);
            }
        }

        void rtrim(std::string& target, const std::string& str) {
            if (!target.empty()) {
                const size_t pos = target.find_last_not_of(str);
                target           = target.substr(0, pos + 1);
            }
        }

        void trim(std::string& target, const std::string& str) {
            internal::ltrim(target, str);
            internal::rtrim(target, str);
        }

        void trim_spaces(std::string& target) {
            internal::trim(target, " 　");  // Half-width space and full-width space
        }
    }

    Pencil::Pencil(std::string_view _name) {
        set_name(_name);
    }

    Pencil::Pencil(std::string_view _name, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
        set_name(_name);
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    std::optional<Pencil> Pencil::parse(std::string_view str) {
        Pencil pencil;

        const auto index = str.find('/');
        if (index == std::string::npos) {
            msg::print(msg::Type::Error, "Pencil::parse", "No separator in string");
            return std::nullopt;
        }

        std::string pencil_name = std::string(str.substr(0, index));
        internal::trim_spaces(pencil_name);
        if (pencil_name.empty()) {
            msg::print(msg::Type::Error, "Pencil::parse", "Empty pencil name");
            return std::nullopt;
        }

        STRCPY(pencil.name, pencil_name.c_str());

        try {
            std::string color_string = std::string(str.substr(index + 1, str.length() - index));
            internal::trim_spaces(color_string);
            internal::trim(color_string, "()");
            std::stringstream stream{color_string};
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
                msg::print(msg::Type::Error, "Pencil::parse", "Invalid pencil color");
                return std::nullopt;
            }

            return pencil;
        } catch (const std::exception& e) {
            msg::print(msg::Type::Error, "Pencil::parse", std::string(e.what()));
            return std::nullopt;
        }
    }

    bool Pencil::set_name(std::string_view _name) {
        const bool is_valid_length = _name.length() <= max_name_length;

        if (is_valid_length) {
            STRCPY(name, _name.data());
        } else {
            const auto str = std::string(_name.substr(0, max_name_length)) + '\0';
            STRCPY(name, str.data());
            msg::print(msg::Type::Warning, "Pencil::set_name", "Name length exceeds the limit");
        }

        return is_valid_length;
    }

    std::optional<std::string> Pencil::to_string() const {
        if (std::string(name).empty()) {
            msg::print(msg::Type::Error, "Pencil::to_string", "Empty pencil name");
            return std::nullopt;
        }

        std::stringstream stream{""};
        stream << name << "/(" << +r << "," << +g << "," << +b << "," << +a << ")";
        return stream.str();
    }
}
