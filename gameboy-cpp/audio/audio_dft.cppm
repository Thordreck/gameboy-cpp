export module audio:dft;

import std;
import math;

import :utils;

namespace audio
{
    export template <typename Input>
    requires static_sized_range<Input>
        && std::ranges::random_access_range<Input>
        && complex<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto dft(Input&& input) noexcept
    {
        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t size = std::ranges::size(input);

        return make_array<size>([size, &input] (const size_t k) constexpr
        {
            const auto elements = make_array<size>([k, &input] (const size_t i) constexpr
            {
                using namespace std::numbers;

                const auto p
                    = static_cast<sample_type>(2)
                    * pi_v<sample_type>
                    * static_cast<sample_type>(k)
                    * static_cast<sample_type>(i)
                    / static_cast<sample_type>(size);

                const auto input_value = *(std::ranges::begin(input) + i);
                const complex_type twiddle { math::cos(p), -math::sin(p) };

                return input_value * twiddle;
            });

            return std::ranges::fold_left(elements, complex_type{}, std::plus<complex_type>()) / static_cast<sample_type>(size);
        });
    }

    export template <typename Input>
    requires static_sized_range<Input>
        && std::ranges::random_access_range<Input>
        && std::floating_point<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto dft(Input&& input) noexcept
    {
        using sample_type = std::ranges::range_value_t<Input>;
        using complex_type = std::complex<sample_type>;
        constexpr size_t size = std::ranges::size(input);

        const auto complex_input = make_array<size>([&input] (const size_t k) constexpr
        {
            return complex_type { input[k], sample_type {} };
        });

        return dft(complex_input);
    }

}
