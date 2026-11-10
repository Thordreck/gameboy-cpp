
export module math:trigonometry;

import std;
import gcem;

namespace math
{
    export template<std::floating_point T>
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

    export template<std::floating_point T>
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

    export template<std::floating_point T1, std::floating_point T2>
    constexpr [[nodiscard]] auto hypot(const T1 x, const T2 y) noexcept
    {
        if consteval
        {
            return gcem::hypot(x, y);
        }
        else
        {
            if constexpr (std::is_same_v<T1, long double> || std::is_same_v<T2, long double>)
            {
                return std::hypotl(x, y);
            }
            else if constexpr (std::is_same_v<T1, double> || std::is_same_v<T2, double>)
            {
                return std::hypot(x, y);
            }
            else
            {
                return std::hypotf(x, y);
            }
        }
    }
}