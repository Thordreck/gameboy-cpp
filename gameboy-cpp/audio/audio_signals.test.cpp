#include "doctest.h"

import std;
import audio;

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

    template<typename SampleType>
    requires std::is_floating_point_v<SampleType>
    [[nodiscard]] SampleType runtime_sinc(const SampleType x)
    {
        using namespace std::numbers;

        return x == SampleType {}
            ? static_cast<SampleType>(1)
            : std::sin(pi_v<SampleType> * x) / (pi_v<SampleType> * x);
    }

}

TEST_CASE_TEMPLATE("audio.sinc() returns expected values at compile time", T, float, double)
{
    constexpr auto computed_values = []<size_t... Index>(std::index_sequence<Index...>) constexpr
      {
          return std::array { audio::sinc<T>(test_values<T>[Index])... };
      }(std::make_index_sequence<test_values<T>.size()>());

    for (size_t i = 0; i < test_values<T>.size(); i++)
    {
        const T input { test_values<T>[i] };

        CHECK_MESSAGE(
            computed_values[i] == doctest::Approx(runtime_sinc(input)).epsilon(0.001f),
            std::format("Discrepancy found with input {}", input));
    }
}

TEST_CASE_TEMPLATE("math.sinc() returns expected values at runtime", T, float, double)
{
    for (auto x : test_values<T>)
    {
        CHECK_MESSAGE(
            audio::sinc(x) == doctest::Approx(runtime_sinc(x)).epsilon(0.0001f),
            std::format("Discrepancy found with input {}", x));
    }
}
