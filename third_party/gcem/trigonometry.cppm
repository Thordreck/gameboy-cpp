module;
#include <gcem.hpp>

export module gcem:trigonometry;
import std;

namespace gcem_detail
{
    template<typename T>
    consteval T cos(T x) noexcept
    {
        return gcem::cos(x);
    }

    template<typename T>
    consteval T sin(T x) noexcept
    {
        return gcem::sin(x);
    }

}

namespace gcem
{
    export template<typename T>
    requires std::is_floating_point_v<T>
    consteval [[nodiscard]] T cos(const T x) noexcept
    {
        return gcem_detail::cos(x);
    }

    export template<typename T>
    requires std::is_floating_point_v<T>
    consteval [[nodiscard]] T sin(const T x) noexcept
    {
        return gcem_detail::sin(x);
    }
}