export module audio:dac;

import :common;

namespace audio
{
    export template<AudioSample Output, AudioSample Input>
    constexpr Output convert_sample(const Input input)
    {
        static constexpr auto input_range = Input::max - Input::min;
        static constexpr auto output_range = Output::max - Output::min;

        const auto t = (static_cast<Input::underlying_type>(input) - Input::min) / static_cast<float>(input_range);
        return Output { Output::min + output_range * t };
    }

    export constexpr analog_sample dac(const digital_sample input)
    {
        return convert_sample<analog_sample>(input);
    }

}