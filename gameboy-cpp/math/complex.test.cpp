#include "doctest.h"

import std;
import math;

namespace
{
    template <std::floating_point T>
    constexpr std::array  cabs_test_values
    {
        std::complex{ T { 0 },  T { 0 }},   std::complex{ T { 1 },  T { 0 }},    std::complex{ T { 0 },     T { 1 }},
        std::complex{ T { -1 }, T { 0 }},   std::complex{ T { 0 },  T { -1 }},   std::complex{ T { 3 },     T { 4 }},
        std::complex{ T { -3 }, T { 4 }},   std::complex{ T { 3 },  T { -4 }},   std::complex{ T { -3 },    T { -4 }},
        std::complex{ T { 1 },  T { 1 }},   std::complex{ T { -1 }, T { 1 }},    std::complex{ T { 1 },     T { -1 }},
        std::complex{ T { -1 }, T { -1 }},  std::complex{ T { 5 },  T { 12 }},   std::complex{ T { 0.5 },   T { 0.5 }},
        std::complex{ T { 1e10f }, T { 1 }}, std::complex{ T { 1 }, T { 1e10f }}, std::complex{ T { 1e20f }, T { 1e20f }},
        std::complex{ T { 1e-20f }, T { 1e-20f }}, std::complex{std::numeric_limits<T>::max(), T { 0 }},
        std::complex{ T { 0 }, std::numeric_limits<T>::max()},
        std::complex{std::numeric_limits<T>::min(), T { 0 }},
        std::complex{ T { 0 }, std::numeric_limits<T>::min()},
        std::complex{std::numeric_limits<T>::denorm_min(), T { 0 }},
        std::complex{ T { 0 }, std::numeric_limits<T>::denorm_min()},
    };
}

TEST_CASE_TEMPLATE("math.math::cabs() returns values similar to std::abs() at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
    {
        return std::array{math::cabs(cabs_test_values<T>[Index])...};
    }
    (std::make_index_sequence<cabs_test_values<T>.size()>());

    for (size_t i = 0; i < cabs_test_values<T>.size(); i++)
    {
        const auto input { cabs_test_values<T>[i] };
        const auto computed { computed_values[i] };
        const auto expected { std::abs(input) };

        CHECK_MESSAGE(
            computed == doctest::Approx(expected).epsilon(0.001f),
            std::format("Discrepancy found with input ({}, {}i) at index {}", input.real(), input.imag(), i));
    }
}

TEST_CASE_TEMPLATE("math.math::cabs() returns values similar to std::abs() at runtime", T, float, double)
{
    for (auto x : cabs_test_values<T>)
    {
        CHECK_MESSAGE(
            math::cabs(x) == doctest::Approx(std::abs(x)).epsilon(0.0001f),
            std::format("Discrepancy found with input ({}, {}i)", x.real(), x.imag()));
    }
}
