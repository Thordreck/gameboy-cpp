#include "doctest.h"

import std;
import audio;

namespace
{
    constexpr std::array blackman_table
    {
        -0.000000, 0.000898, 0.003632, 0.008313, 0.015121, 0.024293, 0.036108, 0.050870, 0.068887, 0.090453,
        0.115824, 0.145195, 0.178685, 0.216316, 0.258001, 0.303528, 0.352565, 0.404646, 0.459183, 0.515473, 0.572709,
        0.630000, 0.686393, 0.740895, 0.792504, 0.840233, 0.883142, 0.920364, 0.951130, 0.974796, 0.990861, 0.998981,
        0.998981, 0.990861, 0.974796, 0.951130, 0.920364, 0.883142, 0.840233, 0.792504, 0.740895, 0.686393, 0.630000,
        0.572709, 0.515473, 0.459183, 0.404646, 0.352565, 0.303528, 0.258001, 0.216316, 0.178685, 0.145195, 0.115824,
        0.090453, 0.068887, 0.050870, 0.036108, 0.024293, 0.015121, 0.008313, 0.003632, 0.000898, -0.000000
    };
}

TEST_CASE_TEMPLATE("audio.Blackman window generates expected values at compile time", T, float, double)
{
    constexpr auto generated = audio::blackman_window<T, blackman_table.size()>();

    for (size_t i = 0; i < generated.size(); i++)
    {
        CHECK(generated[i] == doctest::Approx(blackman_table[i]).epsilon(0.001f));
    }
}

TEST_CASE_TEMPLATE("audio.Blackman window generates expected values at runtime", T, float, double)
{
    const auto generated = audio::blackman_window<T, blackman_table.size()>();

    for (size_t i = 0; i < generated.size(); i++)
    {
        CHECK(generated[i] == doctest::Approx(blackman_table[i]).epsilon(0.001f));
    }
}
