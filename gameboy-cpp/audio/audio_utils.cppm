
export module audio:utils;
import std;

namespace audio
{
    export template <std::size_t N, std::invocable<size_t> Generator>
    constexpr auto make_array(Generator&& generator)
    {
        return [&]<size_t... Index>(std::index_sequence<Index...>) constexpr
          {
              return std::array { generator(Index)... };
          }
        (std::make_index_sequence<N>());
    }

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

    export template<static_sized_range T>
    struct static_range_size
    {
        static constexpr auto value { static_range_size_impl<std::remove_cvref_t<T>>::value };
    };

    export template<static_sized_range T>
    constexpr auto static_range_size_v { static_range_size<T>::value };

    export template<std::integral auto N>
    concept power_of_two = std::has_single_bit(N);

    export template<static_sized_range Range>
    constexpr [[nodiscard]] auto normalize(const Range& range)
    {
        constexpr size_t input_size = std::ranges::size(range);

        return make_array<input_size>([&range] (const size_t i) constexpr
        {
            using range_element_t = std::ranges::range_value_t<Range>;
            constexpr range_element_t normalization { range_element_t { 1 } / static_cast<range_element_t>(input_size) };

            return range[i] * normalization;
        });
    }

}