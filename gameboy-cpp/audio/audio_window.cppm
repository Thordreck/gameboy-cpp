
export module audio:window;
import std;

namespace audio
{
    export template<typename SampleType, size_t N>
    requires (N > 1) && std::is_floating_point_v<SampleType>
    [[nodiscard]] constexpr std::array<SampleType, N> blackman_window()
    {
        size_t n {};
        std::array<SampleType, N> window {};
        std::ranges::generate(window.begin(), window.end(), [n] mutable
        {
            const SampleType w = 2.0f * std::numbers::pi_v<SampleType> * n++ / (N - 1);
            return 0.42f - 0.5f * std::cos(w) + 0.08f * std::cos(2 * w);
        });

        return window;
    }

    export template<size_t N>
    [[nodiscard]] constexpr auto blackman_window_f() { return blackman_window<float, N>(); }

    export template<size_t N>
    [[nodiscard]] constexpr auto blackman_window_d() { return blackman_window<double, N>(); }

}