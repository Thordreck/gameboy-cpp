#include "doctest.h"

import std;
import audio;

namespace
{
    struct test_case
    {
        test_case(
            const audio::digital_sample::underlying_type input,
            const audio::analog_sample::underlying_type expected_output)
                : input { input }
                , expected_output { expected_output }
        {}

        audio::digital_sample input;
        audio::analog_sample expected_output;
    };
}

TEST_CASE("audio.DAC generates expected result")
{
    const auto test = GENERATE(
        test_case{0, -1},
        test_case{0xF, 1},
        test_case{0xC, 0.6},
        test_case{0x8, 0.06}
        );

    const auto computed_value = audio::apply_dac(test.input);
    CHECK(computed_value.data() == doctest::Approx(test.expected_output.data()).epsilon(0.1f));
}
