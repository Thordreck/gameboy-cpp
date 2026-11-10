module;
#include <gcem.hpp>

export module gcem:common;
import std;

namespace gcem_detail
{
    template<std::floating_point T>
    consteval T sqrt(T x) noexcept
    {
        return gcem::sqrt(x);
    }

}

namespace gcem
{
    export template<std::floating_point T>
    consteval [[nodiscard]] T sqrt(const T x) noexcept
    {
        return gcem_detail::sqrt(x);
    }

}
