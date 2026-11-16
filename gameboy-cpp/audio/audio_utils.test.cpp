#include "doctest.h"

import std;
import math;
import audio;

namespace
{
    template<typename T, size_t N>
    requires std::floating_point<T> || math::complex<T>
    struct test_values
    {
        using value_t = T;
        static constexpr auto size = N;

        std::array<T, N> input;
        std::array<T, N> expected;
    };

    using namespace std::complex_literals;

    constexpr test_values case_1 { std::array { 3.0f, 4.0f }, { 1.5f, 2.0f } };
    constexpr test_values case_2 { std::array {4.0f, 8.0f, 12.0f, 16.0f}, {1.0f, 2.0f, 3.0f, 4.0f} };
    constexpr test_values case_3 { std::array {-8.0f, 0.0f, 8.0f, 16.0f}, {-2.0f, 0.0f, 2.0f, 4.0f} };
    constexpr test_values case_4
    {
        std::array { 4.0f + 8.0if, 8.0f + 12.0if, 12.0f + 16.0if, 16.0f + 20.0if },
        std::array { 1.0f + 2.0if, 2.0f + 3.0if, 3.0f + 4.0if, 4.0f + 5.0if }
    };

    constexpr test_values case_5
    {
        std::array { -8.0f + 4.0if, 0.0f + 0.0if, 8.0f - 4.0if, 16.0f + 8.0if },
        std::array { -2.0f + 1.0if, 0.0f + 0.0if, 2.0f - 1.0if, 4.0f + 2.0if }
    };

    template<test_values Values>
    struct test_case
    {
        using value_t = decltype(Values)::value_t;

        static constexpr auto values = Values;
        static constexpr auto size = decltype(Values)::size;
    };

#define TEST_CASES \
    test_case<case_1>, \
    test_case<case_2>, \
    test_case<case_3>, \
    test_case<case_4>, \
    test_case<case_5>
}

TYPE_TO_STRING_AS("case 1 (float)", test_case<case_1>);
TYPE_TO_STRING_AS("case 2 (float)", test_case<case_2>);
TYPE_TO_STRING_AS("case 3 (float)", test_case<case_3>);
TYPE_TO_STRING_AS("case 4 (complex)", test_case<case_4>);
TYPE_TO_STRING_AS("case 5 (complex)", test_case<case_5>);

TEST_CASE_TEMPLATE("audio.Normalize(input) returns expected value", TestCase, TEST_CASES)
{
    constexpr auto test_case = TestCase::values;
    constexpr auto generated = audio::normalize(test_case.input);

    CHECK_EQ(generated.size(), test_case.expected.size());

    for (size_t i = 0; i < generated.size(); i++)
    {
        if constexpr (math::is_complex_v<typename TestCase::value_t>)
        {
            CHECK_MESSAGE(
                generated[i].real() == doctest::Approx(test_case.expected[i].real()).epsilon(0.001f),
                "Mismatch in real values");

            CHECK_MESSAGE(
                generated[i].imag() == doctest::Approx(test_case.expected[i].imag()).epsilon(0.001f),
                "Mismatch in imaginary values");
        }
        else
        {
            CHECK(generated[i] == doctest::Approx(test_case.expected[i]).epsilon(0.001f));
        }
    }
}

TEST_CASE_TEMPLATE("audio.Normalize(input, output) fills output properly", TestCase, TEST_CASES)
{
    constexpr auto test_case = TestCase::values;

    using value_type = TestCase::value_t;
    constexpr auto test_case_size { TestCase::size };
    std::array<value_type, test_case_size> generated {};

    audio::normalize(test_case.input, generated);

    CHECK_EQ(generated.size(), test_case.expected.size());

    for (size_t i = 0; i < generated.size(); i++)
    {
        if constexpr (math::is_complex_v<value_type>)
        {
            CHECK_MESSAGE(
                generated[i].real() == doctest::Approx(test_case.expected[i].real()).epsilon(0.001f),
                "Mismatch in real values");

            CHECK_MESSAGE(
                generated[i].imag() == doctest::Approx(test_case.expected[i].imag()).epsilon(0.001f),
                "Mismatch in imaginary values");
        }
        else
        {
            CHECK(generated[i] == doctest::Approx(test_case.expected[i]).epsilon(0.001f));
        }
    }
}