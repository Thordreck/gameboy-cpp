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
            for (int i = 0; i < N; ++i)
            {
                samples[0] = audio::stereo { Sample{ left_samples[i] }, Sample { right_samples[i] } };
            }

            expected_output.left = expected_left;
            expected_output.right = expected_right;
        }

        std::array<audio::stereo<Sample>, N> samples;
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

    const auto [left, right] = audio::mix(test.samples[0], test.samples[1], test.samples[2]);

    CHECK(left.data() == doctest::Approx(test.expected_output.left.data()).epsilon(0.1f));
    CHECK(right.data() == doctest::Approx(test.expected_output.right.data()).epsilon(0.1f));
}
