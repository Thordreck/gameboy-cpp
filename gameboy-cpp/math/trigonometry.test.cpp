#include "doctest.h"

import std;
import math;

namespace
{
    template <std::floating_point T>
    constexpr T pi = std::numbers::pi_v<T>;

    template <std::floating_point T>
    constexpr std::array test_values{
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

    template <typename T1, typename T2>
    struct type_pair
    {
        using t1 = T1;
        using t2 = T2;
    };

    template <std::floating_point T1, std::floating_point T2>
    constexpr std::array<std::pair<T1, T2>, 19> hypot_test_values = {
        {
            {T1{0}, T2{0}},
            {T1{3.0}, T2{4.0}},
            {T1{5.0}, T2{12.0}},
            {T1{1.0}, T2{1.0}},
            {T1{1.0}, T2{0.0}},
            {T1{0.0}, T2{1.0}},
            {T1{3.0}, T2{4.0}},
            {T1{-3.0}, T2{4.0}},
            {T1{3.0}, T2{-4.0}},
            {T1{-3.0}, T2{-4.0}},
            {T1{4.0}, T2{3.0}},
            {std::numeric_limits<T1>::max(), T2{0}},
            {T1{0}, std::numeric_limits<T2>::max()},
            {std::numeric_limits<T1>::min(), T2{0}},
            {T1{0}, std::numeric_limits<T2>::min()},
            {std::numeric_limits<T1>::denorm_min(), T2{0}},
            {T1{0}, std::numeric_limits<T2>::denorm_min()},
            {std::numeric_limits<T1>::denorm_min(), std::numeric_limits<T2>::denorm_min()},
        }
    };

#define HYPOT_TYPE_PAIRS \
type_pair<float, float>, \
type_pair<float, double>, \
type_pair<double, float>, \
type_pair<double, double>

    template <std::floating_point T1, std::floating_point T2>
    [[nodiscard]] auto runtime_hypot(const T1 x, const T2 y) noexcept
    {
        if constexpr (std::is_same_v<T1, long double> || std::is_same_v<T2, long double>)
        {
            return std::hypotl(x, y);
        }
        else if constexpr (std::is_same_v<T1, double> || std::is_same_v<T2, double>)
        {
            return std::hypot(x, y);
        }
        else
        {
            return std::hypotf(x, y);
        }
    }
}

TYPE_TO_STRING_AS("float, float", type_pair<float, float>);
TYPE_TO_STRING_AS("float, double", type_pair<float, double>);
TYPE_TO_STRING_AS("double, float", type_pair<double, float>);
TYPE_TO_STRING_AS("double, double", type_pair<double, double>);

TEST_CASE_TEMPLATE("math.math::cos() returns values similar to std::cos() at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
    {
        return std::array{math::cos<T>(test_values<T>[Index])...};
    }(std::make_index_sequence<test_values<T>.size()>());

    for (size_t i = 0; i < test_values<T>.size(); i++)
    {
        const T input{test_values<T>[i]};

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
        return std::array{math::sin<T>(test_values<T>[Index])...};
    }(std::make_index_sequence<test_values<T>.size()>());

    for (size_t i = 0; i < test_values<T>.size(); i++)
    {
        const T input{test_values<T>[i]};

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

TEST_CASE_TEMPLATE("math.math::hypot() returns values similar to std::hypot() at compile time", Types, HYPOT_TYPE_PAIRS)
{
    using T1 = Types::t1;
    using T2 = Types::t2;
    constexpr auto num_tests = hypot_test_values<T1, T2>.size();

    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
    {
        return std::array{math::hypot(hypot_test_values<T1, T2>[Index].first, hypot_test_values<T1, T2>[Index].second)...};
    }(std::make_index_sequence<num_tests>());

    for (size_t i = 0; i < num_tests; i++)
    {
        const auto [x, y] = hypot_test_values<T1, T2>[i];

        CHECK_MESSAGE(
            computed_values[i] == doctest::Approx(runtime_hypot(x, y)).epsilon(0.01f),
            std::format("Discrepancy found with input ({}, {}) at index {}", x, y, i));
    }
}

TEST_CASE_TEMPLATE("math.math::hypot() returns values similar to std::hypot() at runtime", Types, HYPOT_TYPE_PAIRS)
{
    using T1 = Types::t1;
    using T2 = Types::t2;

    for (const auto [x, y] : hypot_test_values<T1, T2>)
    {
        CHECK_MESSAGE(
            math::hypot(x, y) == doctest::Approx(runtime_hypot(x, y)).epsilon(0.01f),
            std::format("Discrepancy found with input ({}, {})", x, y));
    }
}
