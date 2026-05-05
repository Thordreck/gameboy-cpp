
export module utilities:enum_extensions;
import std;

namespace utils::operators
{
    export template<typename T>
    requires std::is_enum_v<T>
    constexpr T operator|(const T lhs, const T rhs)
    {
        return static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs));
    }

}
