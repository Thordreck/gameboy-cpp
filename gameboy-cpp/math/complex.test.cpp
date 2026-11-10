#include "doctest.h"

import std;
import math;

namespace
{
    template <std::floating_point T>
    constexpr T pi = std::numbers::pi_v<T>;

    template <std::floating_point T>
    constexpr std::array cabs_test_values
    {
        std::complex{T{0}, T{0}}, std::complex{T{1}, T{0}}, std::complex{T{0}, T{1}},
        std::complex{T{-1}, T{0}}, std::complex{T{0}, T{-1}}, std::complex{T{3}, T{4}},
        std::complex{T{-3}, T{4}}, std::complex{T{3}, T{-4}}, std::complex{T{-3}, T{-4}},
        std::complex{T{1}, T{1}}, std::complex{T{-1}, T{1}}, std::complex{T{1}, T{-1}},
        std::complex{T{-1}, T{-1}}, std::complex{T{5}, T{12}}, std::complex{T{0.5}, T{0.5}},
        std::complex{T{1e10f}, T{1}}, std::complex{T{1}, T{1e10f}}, std::complex{T{1e20f}, T{1e20f}},
        std::complex{T{1e-20f}, T{1e-20f}}, std::complex{std::numeric_limits<T>::max(), T{0}},
        std::complex{T{0}, std::numeric_limits<T>::max()},
        std::complex{std::numeric_limits<T>::min(), T{0}},
        std::complex{T{0}, std::numeric_limits<T>::min()},
        std::complex{std::numeric_limits<T>::denorm_min(), T{0}},
        std::complex{T{0}, std::numeric_limits<T>::denorm_min()},
    };

    template <std::floating_point T>
    constexpr std::array cexp_test_values
    {
        std::complex{T{0}, T{0}}, std::complex{T{1}, T{0}}, std::complex{T{-1}, T{0}},
        std::complex{T{0}, T{1}}, std::complex{T{0}, T{-1}},
        std::complex{T{1}, T{1}}, std::complex{T{1}, T{-1}}, std::complex{T{-1}, T{1}},
        std::complex{T{-1}, T{-1}},
        std::complex{T{2}, T{0.5}}, std::complex{T{-2}, T{0.5}}, std::complex{T{2}, T{-0.5}},
        std::complex{T{-2}, T{-0.5}},
        std::complex{T{0}, pi<T> / 4 },
        std::complex{T{0}, pi<T> / 2 },
        std::complex{T{0}, pi<T> },
        std::complex{T{0}, 3 * pi<T> / 2 },
        std::complex{T{0}, 2 * pi<T> },
        std::complex{T{1}, T{0.7853981634}},
        std::complex{T{-1}, T{1.5707963268}},
        std::complex{T{10}, T{1}},
        std::complex{T{-10}, T{1}},
        std::complex{T{-100}, T{1}},
        std::complex{T{1e-10}, T{1}},
        std::complex{T{-1e-10}, T{1}},
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
        const auto input{cabs_test_values<T>[i]};
        const auto computed{computed_values[i]};
        const auto expected{std::abs(input)};

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

TEST_CASE_TEMPLATE("math.math::cexp() returns values similar to std::exp() at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
        {
            return std::array{math::cexp(cexp_test_values<T>[Index])...};
        }
    (std::make_index_sequence<cexp_test_values<T>.size()>());

    for (size_t i = 0; i < cexp_test_values<T>.size(); i++)
    {
        const auto input{cexp_test_values<T>[i]};
        const auto computed{computed_values[i]};
        const auto expected{std::exp(input)};

        CHECK_MESSAGE(
            computed.real() == doctest::Approx(expected.real()).epsilon(0.001f),
            std::format("Discrepancy found in real part with input ({}, {}i) at index {}", input.real(), input.imag(), i));

        CHECK_MESSAGE(
            computed.imag() == doctest::Approx(expected.imag()).epsilon(0.001f),
            std::format("Discrepancy found in imaginary part with input ({}, {}i) at index {}", input.real(), input.imag(), i));
    }
}

TEST_CASE_TEMPLATE("math.math::cexp() returns values similar to std::exp() at runtime", T, float, double)
{
    for (auto x : cexp_test_values<T>)
    {
        CHECK_MESSAGE(
            math::cexp(x).real() == doctest::Approx(std::exp(x).real()).epsilon(0.0001f),
            std::format("Discrepancy found in real part with input ({}, {}i)", x.real(), x.imag()));

        CHECK_MESSAGE(
            math::cexp(x).imag() == doctest::Approx(std::exp(x).imag()).epsilon(0.0001f),
            std::format("Discrepancy found in imaginary part with input ({}, {}i)", x.real(), x.imag()));
    }
}
