
#include <string>

namespace ldt::msg {
    enum class Type { Info = 0, Warning, Error };

    void Print(Type type, std::string_view funcName, std::string_view message);
}
