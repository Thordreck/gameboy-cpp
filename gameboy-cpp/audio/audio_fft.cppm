export module audio:fft;

import std;
import math;

import :utils;

namespace audio
{
    export template <typename Input>
        requires static_sized_range<Input>
        && power_of_two<static_range_size_v<Input>>
        && std::ranges::random_access_range<Input>
        && math::complex<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto fft(Input&& time) noexcept
    {
        auto input = time;

        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t input_size = std::ranges::size(input);
        constexpr size_t num_stages = std::countr_zero(input_size);

        // Bit-reversal permutation
        for (size_t i = 0; i < input_size; ++i)
        {
            size_t reversed{};
            size_t value = i;

            for (size_t bit = 0; bit < num_stages; ++bit)
            {
                reversed = (reversed << 1) | (value & 1);
                value >>= 1;
            }

            if (i < reversed)
            {
                std::swap(input[i], input[reversed]);
            }
        }

        // Cooley-Tukey butterflies
        for (size_t stage = 1; stage <= num_stages; ++stage)
        {
            const size_t block_size = size_t{1} << stage;
            const size_t half_block = block_size / 2;

            const sample_type angle =
                -static_cast<sample_type>(2)
                * std::numbers::pi_v<sample_type>
                / static_cast<sample_type>(block_size);

            const std::complex step{math::cos(angle), math::sin(angle)};

            for (size_t block = 0; block < input_size; block += block_size)
            {
                complex_type phase{1, 0};

                for (size_t j = 0; j < half_block; ++j)
                {
                    const auto even = input[block + j];
                    const auto odd = input[block + j + half_block] * phase;

                    input[block + j] = even + odd;
                    input[block + j + half_block] = even - odd;

                    phase = phase * step;
                }
            }
        }

        // Forward normalization
        const sample_type normalization{sample_type{1} / static_cast<sample_type>(input_size)};

        for (auto& value : input)
        {
            value *= normalization;
        }

        return input;
    }

    export template <typename Input>
        requires static_sized_range<Input>
        && power_of_two<static_range_size_v<Input>>
        && std::ranges::random_access_range<Input>
        && std::floating_point<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto fft(Input&& time) noexcept
    {
        using sample_type = std::ranges::range_value_t<Input>;
        using complex_type = std::complex<sample_type>;

        constexpr size_t input_size = std::ranges::size(time);
        constexpr size_t packed_size { input_size / 2 };

        const auto packed = make_array<packed_size>([&time](const size_t i) constexpr
        {
            return complex_type{time[2 * i], time[2 * i + 1]};
        });

        const auto spectrum = fft(packed);
        std::array<complex_type, input_size> result {};

        const auto z = spectrum[0];
        result[0] = { (z.real() + z.imag()) / sample_type{2} };
        result[packed_size] = { (z.real() - z.imag()) / sample_type{2} };

        const auto angle =
            -static_cast<sample_type>(2)
            * std::numbers::pi_v<sample_type>
            / static_cast<sample_type>(input_size);

        const complex_type step{ math::cos(angle), math::sin(angle) };
        complex_type phase = step;

        for (size_t k = 1; k < packed_size; ++k)
        {
            const auto a = spectrum[k];
            const auto b = std::conj(spectrum[packed_size - k]);

            result[k] = (a + b - complex_type { 0, 1 } * phase * (a - b)) / sample_type { 4 };
            result[input_size - k] = std::conj(result[k]);

            phase *= step;
        }

        return result;
    }

    export template <typename Input>
        requires static_sized_range<Input>
        && power_of_two<static_range_size_v<Input>>
        && std::ranges::random_access_range<Input>
        && math::complex<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto ifft(Input&& frequency) noexcept
    {
        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t input_size = std::ranges::size(frequency);

        auto input = frequency;

        constexpr size_t num_stages = std::countr_zero(input_size);

        // Bit-reversal permutation
        for (size_t i = 0; i < input_size; ++i)
        {
            size_t reversed{};
            size_t value = i;

            for (size_t bit = 0; bit < num_stages; ++bit)
            {
                reversed = (reversed << 1) | (value & 1);
                value >>= 1;
            }

            if (i < reversed)
            {
                std::swap(input[i], input[reversed]);
            }
        }

        // Cooley-Tukey butterflies
        for (size_t stage = 1; stage <= num_stages; ++stage)
        {
            const size_t block_size = size_t{1} << stage;
            const size_t half_block = block_size / 2;

            const sample_type angle =
                static_cast<sample_type>(2)
                * std::numbers::pi_v<sample_type>
                / static_cast<sample_type>(block_size);

            const complex_type step { math::cos(angle), math::sin(angle) };

            for (size_t block = 0; block < input_size; block += block_size)
            {
                complex_type phase{1, 0};

                for (size_t j = 0; j < half_block; ++j)
                {
                    const auto even = input[block + j];
                    const auto odd = input[block + j + half_block] * phase;

                    input[block + j] = even + odd;
                    input[block + j + half_block] = even - odd;

                    phase *= step;
                }
            }
        }

        return make_array<input_size>([&](const size_t i) constexpr { return input[i].real(); });
    }


}
