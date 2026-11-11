export module audio:fft;

import std;
import math;

import :utils;

namespace audio
{
    template <typename Range>
    concept StaticPowerOfTwoRange =
        static_sized_range<Range>
        && power_of_two<static_range_size_v<Range>>
        && std::ranges::random_access_range<Range>;

    template <typename Range>
    concept ComplexRange = math::complex<std::ranges::range_value_t<Range>>;

    template <typename Range>
    concept UnsignedIntegralRange = std::unsigned_integral<std::ranges::range_value_t<Range>>;

    template <typename Range>
    concept FloatingPointRange = std::floating_point<std::ranges::range_value_t<Range>>;

    template <typename Map>
    concept IndexPermutationMap = StaticPowerOfTwoRange<Map> && UnsignedIntegralRange<Map>;

    export template <typename Input>
    concept ComplexFFTInput = StaticPowerOfTwoRange<Input> && ComplexRange<Input>;

    export template <typename Input>
    concept RealFFTInput = StaticPowerOfTwoRange<Input> && FloatingPointRange<Input>;

    template<StaticPowerOfTwoRange Range>
    constexpr [[nodiscard]] auto bit_reversal_permutation(const Range& range)
    {
        constexpr size_t input_size = std::ranges::size(range);
        constexpr size_t num_stages = std::countr_zero(input_size);

        return make_array<input_size>([] (const size_t i) constexpr
        {
            size_t reversed {};
            size_t value = i;

            for (size_t bit = 0; bit < num_stages; ++bit)
            {
                reversed = (reversed << 1) | (value & 1);
                value >>= 1;
            }

            return reversed;
        });
    }

    template<RealFFTInput Input>
    constexpr [[nodiscard]] auto pack_as_complex(const Input& input)
    {
        constexpr size_t input_size = std::ranges::size(input);
        constexpr size_t packed_size { input_size / 2 };

        return make_array<packed_size>([&input](const size_t i) constexpr
        {
            return std::complex{input[2 * i], input[2 * i + 1]};
        });
    }

    template<ComplexFFTInput Input>
    constexpr [[nodiscard]] auto unpack_as_real(const Input& input)
    {
        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t packed_size = std::ranges::size(input);
        constexpr size_t input_size { packed_size * 2 };

        std::array<complex_type, input_size> result {};

        const auto z = input[0];
        result[0] = { (z.real() + z.imag()) / sample_type{2} };
        result[packed_size] = { (z.real() - z.imag()) / sample_type{2} };

        constexpr auto angle =
            -static_cast<sample_type>(2)
            * std::numbers::pi_v<sample_type>
            / static_cast<sample_type>(input_size);

        const complex_type step{ math::cos(angle), math::sin(angle) };
        complex_type phase = step;

        for (size_t k = 1; k < packed_size; ++k)
        {
            const auto a = input[k];
            const auto b = std::conj(input[packed_size - k]);

            result[k] = (a + b - complex_type { 0, 1 } * phase * (a - b)) / sample_type { 4 };
            result[input_size - k] = std::conj(result[k]);

            phase *= step;
        }

        return result;
    }

    template <ComplexFFTInput Input, IndexPermutationMap Permutation>
    requires (static_range_size_v<Input> == static_range_size_v<Permutation>)
    constexpr [[nodiscard]] auto cooley_tukey_fft(const Input& input, const Permutation& permutation)
    {
        using complex_type = std::ranges::range_value_t<Input>;
        using sample_type = complex_type::value_type;

        constexpr size_t input_size = std::ranges::size(input);
        constexpr size_t num_stages = std::countr_zero(input_size);

        auto result = make_array<input_size>([&] (const size_t i) constexpr { return input[permutation[i]]; });

        for (size_t stage = 1; stage <= num_stages; ++stage)
        {
            const size_t block_size = size_t{1} << stage;
            const size_t half_block = block_size / 2;
            const sample_type angle = -static_cast<sample_type>(2) * std::numbers::pi_v<sample_type> / static_cast<sample_type>(block_size);
            const std::complex step{math::cos(angle), math::sin(angle)};

            for (size_t block = 0; block < input_size; block += block_size)
            {
                complex_type phase { 1, 0 };

                for (size_t j = 0; j < half_block; ++j)
                {
                    const auto even_index = block + j;
                    const auto odd_index = block + j + half_block;

                    const auto even = result[even_index];
                    const auto odd = result[odd_index] * phase;

                    result[even_index] = even + odd;
                    result[odd_index] = even - odd;

                    phase = phase * step;
                }
            }
        }

        return result;
    }

    export template <ComplexFFTInput Input>
    constexpr [[nodiscard]] auto fft(const Input& input) noexcept
    {
        const auto permutation = bit_reversal_permutation(input);
        const auto transform = cooley_tukey_fft(input, permutation);

        return normalize(transform);
    }

    export template <RealFFTInput Input>
    constexpr [[nodiscard]] auto fft(const Input& input) noexcept
    {
        const auto packed = pack_as_complex(input);
        const auto spectrum = fft(packed);

        return unpack_as_real(spectrum);
    }

    export template <typename Input>
        requires static_sized_range<Input>
        && power_of_two<static_range_size_v<Input>>
        && std::ranges::random_access_range<Input>
        && math::complex<std::ranges::range_value_t<Input>>
    constexpr [[nodiscard]] auto ifft(const Input& frequency) noexcept
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
