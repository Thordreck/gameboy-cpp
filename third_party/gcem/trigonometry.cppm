module;
#include <gcem.hpp>

export module gcem:trigonometry;
import std;

namespace gcem_detail
{
    template<typename T>
    constexpr T cos(T x) noexcept
    {
        return gcem::cos(x);
    }

}

namespace gcem
{
    export template<typename T>
    requires std::is_floating_point_v<T>
    constexpr [[nodiscard]] T cos(const T x) noexcept
    {
        return gcem_detail::cos(x);
    }

}