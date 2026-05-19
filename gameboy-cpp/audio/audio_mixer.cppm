
export module audio:mixer;

import :common;

import std;
import utilities;

namespace audio
{
    export template<size_t N>
    using mixed_analog_sample = utils::constrained<
        analog_sample::underlying_type,
        -static_cast<analog_sample::underlying_type>(N),
        static_cast<analog_sample::underlying_type>(N)>;

    export template<size_t N>
    using mixed_analog_stereo_sample = stereo_sample<mixed_analog_sample<N>>;

    export template<size_t N>
    mixed_analog_stereo_sample<N> mix(
        std::span<const analog_sample, N> left_channels,
        std::span<const analog_sample, N> right_channels)
    {
        const auto left_samples = left_channels | std::views::transform(&analog_sample::raw);
        const auto right_samples = right_channels | std::views::transform(&analog_sample::raw);

        const mixed_analog_sample<N> mixed_left { std::ranges::fold_left(left_samples, 0, std::plus{}) };
        const mixed_analog_sample<N> mixed_right { std::ranges::fold_left(right_samples, 0, std::plus{}) };

        return { mixed_left, mixed_right };
    }

    export template<size_t N>
    requires (N % 2 == 0)
    mixed_analog_stereo_sample<N / 2> mix(const std::span<const analog_sample, N> samples)
    {
        const auto left_channels = samples | std::views::stride(2);
        const auto right_channels = samples | std::views::drop(1) | std::views::stride(2);

        return mix(left_channels, right_channels);
    }

}