#include "doctest.h"

import std;
import audio;

namespace
{
    using namespace std::complex_literals;

    template<std::floating_point SampleType, size_t Size>
    struct dft_test_case
    {
        std::array<SampleType, Size> time {};
        std::array<std::complex<SampleType>, Size> dft {};
    };

    // Test values obtained from https://fouriersolver.com/en/transforms/dft
    constexpr dft_test_case rect
    {
        std::array {  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        std::array { 0.5f + 0if, 0.125f - 0.3017766if, 0.0f + 0if, 0.125f -0.0517766if, 0.0f + 0if, 0.125f + 0.0517766if, 0.0f + 0if, 0.125f + 0.3017766if }
    };

    constexpr dft_test_case impulse
    {
        std::array {  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        std::array { 0.125f + 0if, 0.125f + 0if, 0.125f + 0if, 0.125f + 0if, 0.125f + 0if, 0.125f + 0if, 0.125f + 0if, 0.125f + 0if }
    };

    constexpr dft_test_case square_wave
    {
        std::array {  1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f },
        std::array { 0.0f + 0if, 0.25f - 0.603553if, 0.0f + 0if, 0.25f - 0.1035533if, 0.0f + 0if, 0.25f + 0.1035533if, 0.0f + 0if, 0.25f + 0.603553if }
    };

    constexpr dft_test_case cosine
    {
        std::array {  1.0f, 0.9239f, 0.7071f, 0.3827f, 0.0f, -0.3827f, -0.7071f, -0.9239f },
        std::array { 0.125f + 0if, 0.125f - 0.407751if, 0.125f - 0.1353if, 0.125f - 0.054201if, 0.125f + 0if, 0.125f + 0.05420143if, 0.125f + 0.1353if, 0.125f + 0.40775143if }
    };

    constexpr dft_test_case sine
    {
        std::array {  0.0f, 0.3827f, 0.7071f, 0.9239f, 1.0f, 0.9239f, 0.7071f, 0.3827f },
        std::array { 0.628425f + 0if, -0.2206f + 0if, -0.05177f + 0if, -0.029328f + 0if, -0.024875f + 0if, -0.029328f + 0if, -0.051775f + 0if, -0.22067f + 0if }
    };

    constexpr dft_test_case ramp
    {
        std::array {  0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f },
        std::array { 3.5f + 0if, -0.5f + 1.2071if, -0.5f + 0.5if, -0.5f + 0.207106if, -0.5f + 0if, -0.5f - 0.2071if, -0.5f - 0.5if, -0.5f - 1.2071if }
    };

    template<dft_test_case Case>
    struct test_case
    {
        static constexpr auto test = Case;
    };

#define TEST_CASES \
    test_case<rect>, \
    test_case<impulse>, \
    test_case<square_wave>, \
    test_case<cosine>, \
    test_case<sine>, \
    test_case<ramp>

}

TYPE_TO_STRING_AS("rect", test_case<rect>);
TYPE_TO_STRING_AS("impulse", test_case<impulse>);
TYPE_TO_STRING_AS("square wave", test_case<square_wave>);
TYPE_TO_STRING_AS("cosine", test_case<cosine>);
TYPE_TO_STRING_AS("sine", test_case<sine>);
TYPE_TO_STRING_AS("ramp", test_case<ramp>);

TEST_CASE_TEMPLATE("audio.dft() returns expected values at compile time", TestCase, TEST_CASES)
{
    constexpr auto test_case = TestCase::test;
    constexpr auto computed = audio::dft(test_case.time);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.time[i] };
        const auto expected { test_case.dft[i] };

        CHECK_MESSAGE(
            computed[i].real() == doctest::Approx(expected.real()).epsilon(0.001f),
            std::format("Discrepancy found with on real part {} with input {} at index {}", expected.real(), input, i));

        CHECK_MESSAGE(
            computed[i].imag() == doctest::Approx(expected.imag()).epsilon(0.001f),
            std::format("Discrepancy found with on imaginary part {}i with input {} at index {}", expected.imag(), input, i));
    }
}

TEST_CASE_TEMPLATE("audio.dft() returns expected values at runtime", TestCase, TEST_CASES)
{
    const auto test_case = TestCase::test;
    const auto computed = audio::dft(test_case.time);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.time[i] };
        const auto expected { test_case.dft[i] };

        CHECK_MESSAGE(
            computed[i].real() == doctest::Approx(expected.real()).epsilon(0.001f),
            std::format("Discrepancy found with on real part {} with input {} at index {}", expected.real(), input, i));

        CHECK_MESSAGE(
            computed[i].imag() == doctest::Approx(expected.imag()).epsilon(0.001f),
            std::format("Discrepancy found with on imaginary part {}i with input {} at index {}", expected.imag(), input, i));
    }
}

TEST_CASE_TEMPLATE("audio.idft() returns expected values at compile time", TestCase, TEST_CASES)
{
    constexpr auto test_case = TestCase::test;
    constexpr auto computed = audio::idft(test_case.dft);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.dft[i] };
        const auto expected { test_case.time[i] };

        CHECK_MESSAGE(
            computed[i] == doctest::Approx(expected).epsilon(0.001f),
            std::format("Discrepancy found on value {} with input ({}, {}i) at index {}", expected, input.real(), input.imag(), i));
    }
}

TEST_CASE_TEMPLATE("audio.idft() returns expected values at runtime", TestCase, TEST_CASES)
{
    const auto test_case = TestCase::test;
    const auto computed = audio::idft(test_case.dft);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.dft[i] };
        const auto expected { test_case.time[i] };

        CHECK_MESSAGE(
            computed[i] == doctest::Approx(expected).epsilon(0.001f),
            std::format("Discrepancy found on value {} with input ({}, {}i) at index {}", expected, input.real(), input.imag(), i));
    }
}

TEST_CASE_TEMPLATE("audio.fft() returns expected values at compile time", TestCase, TEST_CASES)
{
    constexpr auto test_case = TestCase::test;
    constexpr auto computed = audio::fft(test_case.time);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.time[i] };
        const auto expected { test_case.dft[i] };

        CHECK_MESSAGE(
            computed[i].real() == doctest::Approx(expected.real()).epsilon(0.001f),
            std::format("Discrepancy found with on real part {} with input {} at index {}", expected.real(), input, i));

        CHECK_MESSAGE(
            computed[i].imag() == doctest::Approx(expected.imag()).epsilon(0.001f),
            std::format("Discrepancy found with on imaginary part {}i with input {} at index {}", expected.imag(), input, i));
    }
}

TEST_CASE_TEMPLATE("audio.fft() returns expected values at runtime", TestCase, TEST_CASES)
{
    const auto test_case = TestCase::test;
    const auto computed = audio::fft(test_case.time);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.time[i] };
        const auto expected { test_case.dft[i] };

        CHECK_MESSAGE(
            computed[i].real() == doctest::Approx(expected.real()).epsilon(0.001f),
            std::format("Discrepancy found with on real part {} with input {} at index {}", expected.real(), input, i));

        CHECK_MESSAGE(
            computed[i].imag() == doctest::Approx(expected.imag()).epsilon(0.001f),
            std::format("Discrepancy found with on imaginary part {}i with input {} at index {}", expected.imag(), input, i));
    }
}

TEST_CASE_TEMPLATE("audio.ifft() returns expected values at compile time", TestCase, TEST_CASES)
{
    constexpr auto test_case = TestCase::test;
    constexpr auto computed = audio::ifft(test_case.dft);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.dft[i] };
        const auto expected { test_case.time[i] };

        CHECK_MESSAGE(
            computed[i] == doctest::Approx(expected).epsilon(0.001f),
            std::format("Discrepancy found on value {} with input ({}, {}i) at index {}", expected, input.real(), input.imag(), i));
    }
}

TEST_CASE_TEMPLATE("audio.ifft() returns expected values at runtime", TestCase, TEST_CASES)
{
    const auto test_case = TestCase::test;
    const auto computed = audio::ifft(test_case.dft);

    for (size_t i = 0; i < computed.size(); i++)
    {
        const auto input { test_case.dft[i] };
        const auto expected { test_case.time[i] };

        CHECK_MESSAGE(
            computed[i] == doctest::Approx(expected).epsilon(0.001f),
            std::format("Discrepancy found on value {} with input ({}, {}i) at index {}", expected, input.real(), input.imag(), i));
    }
}
