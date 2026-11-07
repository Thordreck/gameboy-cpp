
export module math:trigonometry;

import std;
import gcem;

namespace math
{
    export template<typename T>
    requires std::is_floating_point_v<T>
    constexpr [[nodiscard]] T cos(const T x) noexcept
    {
        if consteval
        {
            return gcem::cos(x);
        }
        else
        {
            return std::cos(x);
        }
    }

    export template<typename T>
    requires std::is_floating_point_v<T>
    constexpr [[nodiscard]] T sin(const T x) noexcept
    {
        if consteval
        {
            return gcem::sin(x);
        }
        else
        {
            return std::sin(x);
        }
    }
}