
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

}