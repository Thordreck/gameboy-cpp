export module audio:fft;

import std;
import math;

import :utils;

namespace audio
{
    export template <typename Input>
    concept ComplexFFTRange = StaticPowerOfTwoRange<Input> && ComplexStaticRange<Input>;

    export template <typename Input>
    concept RealFFTRange = StaticPowerOfTwoRange<Input> && FloatingPointStaticRange<Input>;

    template <ComplexStaticRange Input, FloatingPointStaticRange Output>
    requires SameSizeStaticRanges<Input, Output>
    constexpr void real(const Input& input, Output& output)
    {
        std::ranges::transform(input.begin(), input.end(), output.begin(), [] (auto&& complex) constexpr { return complex.real(); });
    }

    template<StaticPowerOfTwoRange Input, StaticPowerOfTwoRange Output>
    requires CompatibleInputOutputStaticRanges<Input, Output>
    constexpr void bit_reversal_permutation(const Input& input, Output& output)
    {
        constexpr size_t input_size = std::ranges::size(input);
        constexpr size_t num_stages = std::countr_zero(input_size);

        std::ranges::copy(input, output.begin());

        for (size_t i = 0; i < input_size; i++)
        {
            size_t reversed {};
            size_t value = i;

            for (size_t bit = 0; bit < num_stages; ++bit)
            {
                reversed = (reversed << 1) | (value & 1);
                value >>= 1;
            }

            if (i < reversed)
            {
                std::swap(output[i], output[reversed]);
            }
        }
    }

    template<RealFFTRange Input, ComplexFFTRange Output>
    requires (static_range_size_v<Output> == static_range_size_v<Input> / 2)
    constexpr void pack_as_complex(const Input& input, Output& output)
    {
        constexpr size_t input_size = std::ranges::size(input);
        constexpr size_t packed_size { input_size / 2 };

        for (size_t i = 0; i < packed_size; i++)
        {
            output[i] = { input[2 * i], input[2 * i + 1] };
        }
    }

    template<ComplexFFTRange Input, ComplexFFTRange Output>
    requires (static_range_size_v<Output> == static_range_size_v<Input> * 2)
    constexpr void unpack_as_real(const Input& input, Output& output)
    {
        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t output_size = std::ranges::size(output);
        constexpr size_t packed_size { output_size / 2 };

        const auto z = input[0];
        output[0] = { (z.real() + z.imag()) / sample_type{2} };
        output[packed_size] = { (z.real() - z.imag()) / sample_type{2} };

        constexpr auto angle =
            -static_cast<sample_type>(2)
            * std::numbers::pi_v<sample_type>
            / static_cast<sample_type>(output_size);

        const complex_type step{ math::cos(angle), math::sin(angle) };
        complex_type phase = step;

        for (size_t k = 1; k < packed_size / 2; ++k)
        {
            const size_t j = packed_size - k;

            const auto a = output[k];
            const auto c = output[j];

            const auto b = std::conj(c);
            const auto d = std::conj(a);

            const auto phase_j = -std::conj(phase);
            const auto result_k = (a + b - complex_type{0, 1} * phase * (a - b)) / sample_type{4};
            const auto result_j = (c + d - complex_type{0, 1} * phase_j * (c - d)) / sample_type{4};

            output[k] = result_k;
            output[j] = result_j;

            output[output_size - k] = std::conj(result_k);
            output[output_size - j] = std::conj(result_j);

            phase *= step;
        }

        const size_t k = packed_size / 2;
        const auto value = output[k];

        output[k] = std::conj(value) / sample_type{2};
        output[output_size - k] = value / sample_type{2};
    }

    template <ComplexFFTRange Input, ComplexFFTRange Output>
    requires CompatibleInputOutputStaticRanges<Input, Output>
    constexpr void cooley_tukey_fft(const Input& input, Output& output, const bool forward)
    {
        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t input_size = std::ranges::size(input);
        constexpr size_t num_stages = std::countr_zero(input_size);

        for (size_t stage = 1; stage <= num_stages; ++stage)
        {
            const size_t block_size = size_t{1} << stage;
            const size_t half_block = block_size / 2;
            const auto direction = static_cast<sample_type>(forward ? -1 : 1);
            const sample_type angle = direction * static_cast<sample_type>(2) * std::numbers::pi_v<sample_type> / static_cast<sample_type>(block_size);
            const std::complex step { math::cos(angle), math::sin(angle) };

            for (size_t block = 0; block < input_size; block += block_size)
            {
                complex_type phase { 1, 0 };

                for (size_t j = 0; j < half_block; ++j)
                {
                    const auto even_index = block + j;
                    const auto odd_index = block + j + half_block;

                    const auto even = input[even_index];
                    const auto odd = input[odd_index] * phase;

                    output[even_index] = even + odd;
                    output[odd_index] = even - odd;

                    phase *= step;
                }
            }
        }
    }

    export template <ComplexFFTRange Input, ComplexFFTRange Output>
    requires CompatibleInputOutputStaticRanges<Input, Output>
    constexpr void fft(const Input& input, Output& output) noexcept
    {
        bit_reversal_permutation(input, output);
        cooley_tukey_fft(output, output, true);
        normalize(output, output);
    }

    export template <ComplexFFTRange Range>
    constexpr void fft(Range& range) noexcept
    {
        fft(range, range);
    }

    export template <ComplexFFTRange Input>
    constexpr [[nodiscard]] auto fft(const Input& input) noexcept
    {
        using range_element_t = std::ranges::range_value_t<Input>;
        constexpr size_t input_size = std::ranges::size(input);

        std::array<range_element_t, input_size> output {};
        fft(input, output);

        return output;
    }

    export template <ComplexStaticRange Input, ComplexFFTRange Output>
    requires CompatibleInputOutputRanges<Input, Output> && (static_range_size_v<Output> == std::bit_ceil(static_range_size_v<Input>))
    constexpr void fft(const Input& input, Output& output) noexcept
    {
        add_padding(input, output);
        fft(output);
    }

    export template <ComplexStaticRange Input>
    constexpr [[nodiscard]] auto fft(const Input& input) noexcept
    {
        using range_value_t = std::ranges::range_value_t<Input>;

        constexpr auto size = std::ranges::size(input);
        constexpr auto power_of_two_size = std::bit_ceil(size);

        std::array<range_value_t, power_of_two_size> output {};
        fft(input, output);

        return output;
    }

    export template <RealFFTRange Input, ComplexFFTRange Output>
    requires SameSizeStaticRanges<Input, Output> && std::ranges::contiguous_range<Output>
    constexpr void fft(const Input& input, Output& output) noexcept
    {
        using range_value_t = std::ranges::range_value_t<Output>;
        constexpr auto size { std::ranges::size(input) / 2 };

        std::span<range_value_t, size> packed { output.begin(), size };

        pack_as_complex(input, packed);
        fft(packed);
        unpack_as_real(packed, output);
    }

    export template <RealFFTRange Input>
    constexpr [[nodiscard]] auto fft(const Input& input) noexcept
    {
        using range_value_t = std::ranges::range_value_t<Input>;
        constexpr auto size = std::ranges::size(input);

        std::array<std::complex<range_value_t>, size> output {};
        fft(input, output);

        return output;
    }

    export template <FloatingPointStaticRange Input>
    constexpr [[nodiscard]] auto fft(const Input& input) noexcept
    {
        using range_value_t = std::ranges::range_value_t<Input>;

        constexpr auto size = std::ranges::size(input);
        constexpr auto power_of_two_size = std::bit_ceil(size);

        std::array<range_value_t, power_of_two_size> output {};
        add_padding(input, output);

        return fft(output);
    }

    export template <ComplexFFTRange Input>
    constexpr [[nodiscard]] auto ifft(const Input& input) noexcept
    {
        using input_element_t = std::ranges::range_value_t<Input>;
        using result_element_t = input_element_t::value_type;
        constexpr size_t input_size = std::ranges::size(input);

        std::array<input_element_t, input_size> transform {};
        std::array<result_element_t, input_size> result {};

        bit_reversal_permutation(input, transform);
        cooley_tukey_fft(transform, transform, false);
        real(transform, result);

        return result;
    }

}
