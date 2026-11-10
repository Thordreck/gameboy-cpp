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

    template<std::floating_point T>
    consteval T log(T x) noexcept
    {
        return gcem::log(x);
    }

}

namespace gcem
{
    export template<std::floating_point T>
    consteval [[nodiscard]] T sqrt(const T x) noexcept
    {
        return gcem_detail::sqrt(x);
    }

    export template<std::floating_point T>
    consteval [[nodiscard]] T log(const T x) noexcept
    {
        return gcem_detail::log(x);
    }

}
