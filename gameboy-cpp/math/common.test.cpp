#include "doctest.h"

import std;
import math;

namespace
{
    template <std::floating_point T>
    constexpr std::array<T, 12> log_test_values =
    {{
        T{1}, T{2}, T{0.5}, T{10}, T{100},
        T{1.0 - 1e-10}, T{1.0 + 1e-10}, T{1.0 - 1e-15}, T{1.0 + 1e-15}, T{1e10},
        std::numeric_limits<T>::min(), std::numeric_limits<T>::max(),
    }};

}

TEST_CASE_TEMPLATE("math.math::log() returns values similar to std::log() at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
    {
        return std::array{math::log(log_test_values<T>[Index])...};
    }
    (std::make_index_sequence<log_test_values<T>.size()>());

    for (size_t i = 0; i < log_test_values<T>.size(); i++)
    {
        const auto input { log_test_values<T>[i] };
        const auto computed { computed_values[i] };
        const auto expected { std::log(input) };

        CHECK_MESSAGE(
            computed == doctest::Approx(expected).epsilon(0.001f),
            std::format("Discrepancy found with input {} at index {}", input, i));
    }
}

TEST_CASE_TEMPLATE("math.math::log() returns values similar to std::log() at runtime", T, float, double)
{
    for (auto x : log_test_values<T>)
    {
        CHECK_MESSAGE(
            math::log(x) == doctest::Approx(std::log(x)).epsilon(0.0001f),
            std::format("Discrepancy found with input {}", x));
    }
}