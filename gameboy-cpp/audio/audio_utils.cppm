
export module audio:utils;

import std;
import math;

namespace audio
{
    export template<typename T>
    concept static_sized_range
        = std::ranges::sized_range<T>
        && requires(T& t)
    {
        typename std::integral_constant<decltype(std::ranges::size(t)), std::ranges::size(t)>;
    };

    // Adapted from https://www.thinkcell.com/en/career/devblog/compile-time-sizes-for-range-adaptors
    template<static_sized_range T>
    struct static_range_size_impl;

    template<typename T, size_t N>
    struct static_range_size_impl<std::array<T, N>> : std::integral_constant<size_t, N> {};

    template<typename T, size_t N>
    struct static_range_size_impl<std::span<T, N>> : std::integral_constant<size_t, N> {};

    export template<static_sized_range T>
    struct static_range_size
    {
        static constexpr auto value { static_range_size_impl<std::remove_cvref_t<T>>::value };
    };

    export template<static_sized_range T>
    constexpr auto static_range_size_v { static_range_size<T>::value };

    export template<std::integral auto N>
    concept power_of_two = std::has_single_bit(N);

    template <typename Range>
    concept StaticRange = static_sized_range<Range> && std::ranges::random_access_range<Range>;

    template <typename Range>
    concept ComplexRange = math::complex<std::ranges::range_value_t<Range>>;

    template <typename Range>
    concept ComplexStaticRange = StaticRange<Range> && ComplexRange<Range>;

    template <typename Range>
    concept UnsignedIntegralRange = std::unsigned_integral<std::ranges::range_value_t<Range>>;

    template <typename Range>
    concept UnsignedIntegralStaticRange = StaticRange<Range> && UnsignedIntegralRange<Range>;

    template <typename Range>
    concept FloatingPointRange = std::floating_point<std::ranges::range_value_t<Range>>;

    template <typename Range>
    concept FloatingPointStaticRange = StaticRange<Range> && FloatingPointRange<Range>;

    template <typename Range>
    concept StaticPowerOfTwoRange = StaticRange<Range> && power_of_two<static_range_size_v<Range>>;

    template <typename Input, typename Output>
    concept SameSizeStaticRanges
        = StaticRange<Input> && StaticRange<Output>
        && (static_range_size_v<Input> == static_range_size_v<Output>);

    template <typename Input, typename Output>
    concept CompatibleInputOutputRanges
        = std::convertible_to<std::ranges::range_value_t<Input>, std::ranges::range_value_t<Output>>;

    template <typename Input, typename Output>
    concept CompatibleInputOutputStaticRanges
        = SameSizeStaticRanges<Input, Output>
        && CompatibleInputOutputRanges<Input, Output>;

    export template <std::size_t N, std::invocable<size_t> Generator>
    constexpr auto make_array(Generator&& generator)
    {
        using value_type = std::remove_cvref_t<std::invoke_result_t<Generator&, std::size_t>>;

        std::array<value_type, N> result {};

        for (std::size_t i = 0; i < N; ++i)
        {
            result[i] = generator(i);
        }

        return result;
    }

    export template<typename Input, typename Output>
    requires (FloatingPointStaticRange<Input> || ComplexStaticRange<Input>)
        && (FloatingPointStaticRange<Output> || ComplexStaticRange<Output>)
        && CompatibleInputOutputStaticRanges<Input, Output>
    constexpr void normalize(const Input& input, Output& output)
    {
        using input_element_t = std::ranges::range_value_t<Input>;

        constexpr size_t size = std::ranges::size(input);
        constexpr input_element_t normalization { input_element_t { 1 } / static_cast<input_element_t>(size) };

        for (size_t i = 0; i < size; i++)
        {
            output[i] = input[i] * normalization;
        }
    }

    export template<typename Range>
    requires FloatingPointStaticRange<Range> || ComplexStaticRange<Range>
    constexpr [[nodiscard]] auto normalize(const Range& input)
    {
        using range_element_t = std::ranges::range_value_t<Range>;
        constexpr size_t input_size = std::ranges::size(input);

        std::array<range_element_t, input_size> output {};
        normalize(input, output);

        return output;
    }

    export template<static_sized_range Input, static_sized_range Output>
    requires (static_range_size_v<Output> > static_range_size_v<Input>) && CompatibleInputOutputRanges<Input, Output>
    constexpr auto add_padding(const Input& input, Output& output)
    {
        constexpr auto input_size = static_range_size_v<Input>;
        using range_value_t = std::ranges::range_value_t<Output>;

        std::ranges::copy(input, output.begin());
        std::ranges::fill(std::next(output.begin(), input_size) , output.end(), range_value_t {});
    }

}