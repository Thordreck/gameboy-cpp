
export module math:common;

import std;
import gcem;

namespace math
{
    export template<std::floating_point T>
    constexpr [[nodiscard]] T log(const T x) noexcept
    {
        if consteval
        {
            return gcem::log(x);
        }
        else
        {
            return std::log(x);
        }
    }

    export template<std::floating_point T>
    constexpr [[nodiscard]] T fma(const T x, const T y, const T z) noexcept
    {
        if consteval
        {
            return x * y + z;
        }
        else
        {
            return std::fma(x, y, z);
        }
    }

}