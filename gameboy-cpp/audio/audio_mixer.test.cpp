#include "doctest.h"

import std;
import audio;

namespace
{
    template<audio::AudioSample Sample, size_t N>
    struct test_case
    {
        using sample_t = Sample::underlying_type;

        test_case(
            const std::span<const sample_t, N> left_samples,
            const std::span<const sample_t, N> right_samples,
            const sample_t expected_left,
            const sample_t expected_right)
        {
            std::ranges::transform(
                left_samples,
                left_channels.begin(),
                [](const auto sample) { return Sample { sample }; });

            std::ranges::transform(
                right_samples,
                right_channels.begin(),
                [](const auto sample) { return Sample { sample }; });

            expected_output.left = expected_left;
            expected_output.right = expected_right;
        }

        std::array<Sample, N> left_channels;
        std::array<Sample, N> right_channels;

        audio::mixed_stereo_sample<Sample, N> expected_output;
    };
}

TEST_CASE("audio.Mixer generates expected result")
{
    const auto test = GENERATE(
        test_case<audio::analog_sample, 3>
        {
            std::array{ 0.0f, 0.0f, 0.0f },
            std::array{ 0.0f, 0.0f, 0.0f },
            0,
            0
        },
        test_case<audio::analog_sample, 3>
        {
            std::array{ -1.0f, 1.0f, 0.5f },
            std::array{ 1.0f, 1.0f, 1.0f },
            0.5f,
            3
        },
        test_case<audio::analog_sample, 3>
        {
            std::array{ 0.0f, 1.0f, 0.5f },
            std::array{ -1.0f, -1.0f, -1.0f },
            1.5f,
            -3
        }
        );

    const auto [left, right] = audio::mix(std::span { test.left_channels }, std::span { test.right_channels });

    CHECK(left.raw() == doctest::Approx(test.expected_output.left.raw()).epsilon(0.1f));
    CHECK(right.raw() == doctest::Approx(test.expected_output.right.raw()).epsilon(0.1f));
}
