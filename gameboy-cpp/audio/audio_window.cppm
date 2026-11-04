
export module audio:window;
import std;

namespace audio
{
    export template<size_t N>
    requires (N > 1)
    [[nodiscard]] constexpr std::array<float, N> blackman_window()
    {
        size_t n {};
        std::array<float, N> window {};
        std::ranges::generate(window.begin(), window.end(), [n] mutable
        {
            const float w = 2.0f * std::numbers::pi_v<float> * n++ / (N - 1);
            return 0.42f - 0.5f * std::cos(w) + 0.08f * std::cos(2 * w);
        });

        return window;
    }

}