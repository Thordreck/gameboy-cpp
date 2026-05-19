#include "doctest.h"

import std;
import audio;

namespace
{
    template<size_t N>
    struct test_case
    {
        test_case(
            const std::span<const audio::analog_sample::underlying_type, N> left_samples,
            const std::span<const audio::analog_sample::underlying_type, N> right_samples,
            const audio::analog_sample::underlying_type expected_left,
            const audio::analog_sample::underlying_type expected_right)
        {
            std::ranges::transform(
                left_samples,
                left_channels.begin(),
                [](const auto sample) { return audio::analog_sample { sample }; });

            std::ranges::transform(
                right_samples,
                right_channels.begin(),
                [](const auto sample) { return audio::analog_sample { sample }; });

            expected_output.left = expected_left;
            expected_output.right = expected_right;
        }

        std::array<audio::analog_sample, N> left_channels;
        std::array<audio::analog_sample, N> right_channels;

        audio::mixed_analog_stereo_sample<N> expected_output;
    };
}

TEST_CASE("audio.Mixer generates expected result")
{
    const auto test = GENERATE(
        test_case<3>{ std::array{ 0.0f, 0.0f, 0.0f }, std::array{ 0.0f, 0.0f, 0.0f }, 0, 0 },
        test_case<3>{ std::array{ -1.0f, 1.0f, 0.5f }, std::array{ 1.0f, 1.0f, 1.0f }, 0.5f, 3 },
        test_case<3>{ std::array{ 0.0f, 1.0f, 0.5f }, std::array{ -1.0f, -1.0f, -1.0f }, 1.5f, -3 }
        );

    const auto [left, right] = audio::mix<3>(test.left_channels, test.right_channels);

    CHECK(left.raw() == doctest::Approx(test.expected_output.left.raw()).epsilon(0.1f));
    CHECK(right.raw() == doctest::Approx(test.expected_output.right.raw()).epsilon(0.1f));
}
