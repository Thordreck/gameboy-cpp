
export module math:complex;

import std;
import :trigonometry;

namespace math
{
    template<typename>
    struct is_complex : std::false_type {};

    template<typename T>
    struct is_complex<std::complex<T>> : std::true_type {};

    export template<typename T>
    constexpr bool is_complex_v = is_complex<T>::value;

    export template<typename T>
    concept complex = is_complex_v<T>;

    export template<complex T>
    constexpr [[nodiscard]] auto cabs(const T x) noexcept
    {
        if consteval
        {
            return hypot(x.real(), x.imag());
        }
        else
        {
            return std::abs(x);
        }
    }

}
