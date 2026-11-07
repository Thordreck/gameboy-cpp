#include "doctest.h"

import std;
import math;

namespace
{
    template<typename T>
    requires std::is_floating_point_v<T>
    constexpr T pi = std::numbers::pi_v<T>;

    template<typename T>
    requires std::is_floating_point_v<T>
    constexpr std::array test_values {
        T{0}, T{1e-7}, T{-1e-7}, T{1e-4}, T{-1e-4},
        pi<T> / T{6}, -pi<T> / T{6}, pi<T> / T{4}, -pi<T> / T{4}, pi<T> / T{3}, -pi<T> / T{3},
        pi<T> / T{2}, -pi<T> / T{2}, pi<T>, -pi<T>,
        T{3} * pi<T> / T{2}, -T{3} * pi<T> / T{2}, T{2} * pi<T>, -T{2} * pi<T>,
        T{10} * pi<T>, -T{10} * pi<T>,
        T{100} * pi<T>, -T{100} * pi<T>,
        T{1000} * pi<T>, -T{1000} * pi<T>,
        T{10000} * pi<T>, -T{10000} * pi<T>,
        T{1e5}, T{-1e5},
    };
}

TEST_CASE_TEMPLATE("math.math::cos() returns values similar to std::cos() at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
      {
          return std::array { math::cos<T>(test_values<T>[Index])... };
      }(std::make_index_sequence<test_values<T>.size()>());

    for (size_t i = 0; i < test_values<T>.size(); i++)
    {
        const T input { test_values<T>[i] };

        CHECK_MESSAGE(
            computed_values[i] == doctest::Approx(std::cos(input)).epsilon(0.001f),
            std::format("Discrepancy found with input {}", input));
    }
}

TEST_CASE_TEMPLATE("math.math::cos() returns values similar to std::cos() at runtime", T, float, double)
{
    for (auto x : test_values<T>)
    {
        CHECK_MESSAGE(
            math::cos(x) == doctest::Approx(std::cos(x)).epsilon(0.0001f),
            std::format("Discrepancy found with input {}", x));
    }
}

TEST_CASE_TEMPLATE("math.math::sin() returns values similar to std::sin() at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
      {
          return std::array { math::sin<T>(test_values<T>[Index])... };
      }(std::make_index_sequence<test_values<T>.size()>());

    for (size_t i = 0; i < test_values<T>.size(); i++)
    {
        const T input { test_values<T>[i] };

        CHECK_MESSAGE(
            computed_values[i] == doctest::Approx(std::sin(input)).epsilon(0.01f),
            std::format("Discrepancy found with input {}", input));
    }
}

TEST_CASE_TEMPLATE("math.math::cos() returns values similar to std::cos() at runtime", T, float, double)
{
    for (auto x : test_values<T>)
    {
        CHECK_MESSAGE(
            math::sin(x) == doctest::Approx(std::sin(x)).epsilon(0.001f),
            std::format("Discrepancy found with input {}", x));
    }
}
