
export module audio:mixer;

import :common;

import std;
import utilities;

namespace audio
{
    export template<AudioSample Sample, size_t N>
    using mixed_sample = utils::constrained<typename Sample::underlying_type, N * Sample::min, N * Sample::max>;

    export template<AudioSample Sample, size_t N>
    using mixed_stereo_sample = stereo<mixed_sample<Sample, N>>;

    export template<AudioSample Sample, size_t N>
    mixed_stereo_sample<Sample, N> mix(std::span<const Sample, N> left_channel, std::span<const Sample, N> right_channel)
    {
        using sample_t = Sample::underlying_type;

        const auto left_samples = left_channel | std::views::transform([] (auto s) { return static_cast<sample_t>(s); });
        const auto right_samples = right_channel | std::views::transform([] (auto s) { return static_cast<sample_t>(s); });

        const mixed_sample<Sample, N> mixed_left { std::ranges::fold_left(left_samples, 0, std::plus{}) };
        const mixed_sample<Sample, N> mixed_right { std::ranges::fold_left(right_samples, 0, std::plus{}) };

        return { mixed_left, mixed_right };
    }

    export template<AudioSample Sample, size_t N>
    requires (N % 2 == 0)
    mixed_stereo_sample<Sample, N / 2> mix(const std::span<const Sample, N> samples)
    {
        const auto left_channels = samples | std::views::stride(2);
        const auto right_channels = samples | std::views::drop(1) | std::views::stride(2);

        return mix(left_channels, right_channels);
    }

}