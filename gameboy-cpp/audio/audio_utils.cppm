
export module audio:utils;

import std;
import math;

namespace audio
{
    export template<typename SampleType>
    requires std::is_floating_point_v<SampleType>
    constexpr [[nodiscard]] SampleType sinc(const SampleType x) noexcept
    {
        using namespace std::numbers;

        return x == SampleType {}
            ? static_cast<SampleType>(1)
            : math::sin(pi_v<SampleType> * x) / (pi_v<SampleType> * x);
    }

    export template <std::size_t N, std::invocable<size_t> Generator>
    constexpr auto make_array(Generator&& generator)
    {
        return [&]<size_t... Index>(std::index_sequence<Index...>) constexpr
          {
              return std::array { generator(Index)... };
          }(std::make_index_sequence<N>());
    }

    template<typename>
    struct is_complex : std::false_type {};

    template<typename T>
    struct is_complex<std::complex<T>> : std::true_type {};

    export template<typename T>
    constexpr bool is_complex_v = is_complex<T>::value;

    export template<typename T>
    concept complex = is_complex_v<T>;

    export template<typename T>
    concept static_sized_range = std::ranges::sized_range<T> && requires(T& t) { typename std::integral_constant<size_t, std::ranges::size(t)>; };

}