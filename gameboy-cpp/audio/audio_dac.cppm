export module audio:dac;

import :common;

namespace audio
{
    export constexpr analog_sample dac(const digital_sample input)
    {
        using analog_sample_t = analog_sample::underlying_type;

        static constexpr auto digital_range = digital_sample::max - digital_sample::min;
        static constexpr auto analog_range = analog_sample::max - analog_sample::min;

        const float t = (static_cast<analog_sample_t>(input.raw()) - digital_sample::min) / digital_range;
        return analog_sample { analog_sample::min + analog_range * t };
    }

}