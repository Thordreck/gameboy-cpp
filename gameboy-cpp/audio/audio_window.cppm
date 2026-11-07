
export module audio:window;

import std;
import math;

namespace audio
{
    export template<typename SampleType, size_t N>
    requires (N > 1) && std::is_floating_point_v<SampleType>
    [[nodiscard]] constexpr std::array<SampleType, N> blackman_window() noexcept
    {
        const auto generate_element = [] (const size_t i) constexpr
        {
            const SampleType w = 2.0f * std::numbers::pi_v<SampleType> * i / (N - 1);
            return 0.42f - 0.5f * math::cos(w) + 0.08f * math::cos(2 * w);
        };

        const auto generate_sequence = [&generate_element]<size_t... Index>(std::index_sequence<Index...>) constexpr
        {
            return std::array<SampleType, N> { generate_element(Index)... };
        };

        return generate_sequence(std::make_index_sequence<N>());
    }

    export template<size_t N>
    [[nodiscard]] constexpr auto blackman_window_f() noexcept { return blackman_window<float, N>(); }

    export template<size_t N>
    [[nodiscard]] constexpr auto blackman_window_d() noexcept { return blackman_window<double, N>(); }

}