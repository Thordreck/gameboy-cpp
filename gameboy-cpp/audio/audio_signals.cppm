
export module audio:signals;

import std;
import math;

import :dft;
import :utils;

namespace audio
{
    export template<std::floating_point SampleType>
    constexpr [[nodiscard]] SampleType sinc(const SampleType x) noexcept
    {
        using namespace std::numbers;

        return x == SampleType {}
            ? static_cast<SampleType>(1)
            : math::sin(pi_v<SampleType> * x) / (pi_v<SampleType> * x);
    }

    export template <typename Input>
    requires static_sized_range<Input>
        && std::ranges::random_access_range<Input>
        && std::floating_point<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto rceps(Input&& input)
    {
        constexpr auto size = std::ranges::size(input);

        const auto transform = dft(input);
        const auto log_cabs = make_array<size>([&input] (const size_t i) constexpr
        {
            using namespace math;

            const auto input_value = *(std::ranges::begin(input) + i);
            return log(cabs(input_value));
        });

        const auto inverse = idft(log_cabs);
        return make_array<size>([&inverse] (const size_t i) constexpr { return inverse[i].real(); });
    }

    export template <typename Input>
    requires static_sized_range<Input>
        && std::ranges::random_access_range<Input>
        && std::floating_point<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto minimum_phase(Input&& input)
    {
        using sample_type = std::ranges::range_value_t<Input>;
        constexpr auto n { std::ranges::size(input) };

        const auto coefficients = make_array<n>([&input, n] (const size_t i) constexpr
        {
            if (i == 0)
            {
                return input[i];
            }

            const auto nd2 = n / 2;

            if (i < nd2)
            {
                return 2 * input[i];
            }

            if (n % 2 == 0 && i == nd2)
            {
                return input[i];
            }

            return sample_type {};
        });

        const auto transform = dft(coefficients);
        const auto cexp = make_array<n>([&transform, n] (const size_t i) constexpr { return math::cexp(transform[i]); });

        return idft(cexp);
    }

}