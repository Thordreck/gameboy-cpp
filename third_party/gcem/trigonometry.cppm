module;
#include <gcem.hpp>

export module gcem:trigonometry;
import std;

namespace gcem_detail
{
    template<std::floating_point T>
    consteval T cos(const T x) noexcept
    {
        return gcem::cos(x);
    }

    template<std::floating_point T>
    consteval T sin(const T x) noexcept
    {
        return gcem::sin(x);
    }

    template<std::floating_point T1, std::floating_point T2>
    consteval auto hypot(const T1 x, const T2 y) noexcept
    {
        return gcem::hypot(x, y);
    }

}

namespace gcem
{
    export template<std::floating_point T>
    consteval [[nodiscard]] T cos(const T x) noexcept
    {
        return gcem_detail::cos(x);
    }

    export template<std::floating_point T>
    consteval [[nodiscard]] T sin(const T x) noexcept
    {
        return gcem_detail::sin(x);
    }

    export template<std::floating_point T1, std::floating_point T2>
    consteval [[nodiscard]] auto hypot(const T1 x, const T2 y) noexcept
    {
        return gcem_detail::hypot(x, y);
    }

}