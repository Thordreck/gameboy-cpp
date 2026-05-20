
export module audio:common;

import std;
import utilities;

namespace audio
{
    export using digital_sample = utils::constrained<std::uint8_t, 0x0, 0xF>;
    export using analog_sample = utils::constrained<float, -1, 1>;

    export template <typename T>
    struct stereo_sample
    {
        T left;
        T right;
    };

    export template <typename T>
    concept AudioSample = requires(T sample)
    {
        typename T::underlying_type;
        std::is_arithmetic_v<typename T::underlying_type>;

        { T::min } -> std::convertible_to<typename T::underlying_type>;
        { T::max } -> std::convertible_to<typename T::underlying_type>;
        { static_cast<T::underlying_type>(sample) } -> std::convertible_to<typename T::underlying_type>;
    };

}