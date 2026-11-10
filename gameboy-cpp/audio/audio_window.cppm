
export module audio:window;

import std;
import math;

import :utils;

namespace audio
{
    export template<std::floating_point SampleType, size_t N>
    requires (N > 1)
    [[nodiscard]] constexpr std::array<SampleType, N> blackman_window() noexcept
    {
        return make_array<N>([] (const size_t i) constexpr
        {
            const SampleType w = 2.0f * std::numbers::pi_v<SampleType> * i / (N - 1);
            return 0.42f - 0.5f * math::cos(w) + 0.08f * math::cos(2 * w);
        });
    }

    export template<size_t N>
    [[nodiscard]] constexpr auto blackman_window_f() noexcept { return blackman_window<float, N>(); }

    export template<size_t N>
    [[nodiscard]] constexpr auto blackman_window_d() noexcept { return blackman_window<double, N>(); }

}