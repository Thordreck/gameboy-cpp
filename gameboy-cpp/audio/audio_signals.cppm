
export module audio:signals;

import std;
import math;

import :dft;
import :fft;
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
    constexpr [[nodiscard]] auto rceps(const Input& input)
    {
        const auto transform = fft(input);
        const auto log_cabs = make_array<transform.size()>([&transform] (const size_t i) constexpr
        {
            return std::complex { math::log(math::cabs(transform[i])) };
        });

        return ifft(log_cabs);
    }

    export template <typename Input>
    requires static_sized_range<Input>
        && std::ranges::random_access_range<Input>
        && std::floating_point<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto min_phase(const Input& input)
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

        const auto transform = fft(coefficients);
        const auto cexp = make_array<transform.size()>([&transform] (const size_t i) constexpr { return math::cexp(transform[i]); });

        return ifft(cexp);
    }

}