
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
          }(std::make_index_sequence<N>());
    }

    export template<typename T>
    concept static_sized_range = std::ranges::sized_range<T> && requires(T& t) { typename std::integral_constant<size_t, std::ranges::size(t)>; };
}