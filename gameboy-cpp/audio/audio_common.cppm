
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

}