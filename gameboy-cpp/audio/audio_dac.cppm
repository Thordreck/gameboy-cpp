export module audio:dac;

import :common;

namespace audio
{
    export template<AudioSample Output, AudioSample Input>
    [[nodiscard]] constexpr Output convert_sample(const Input input)
    {
        static constexpr auto input_range = Input::max - Input::min;
        static constexpr auto output_range = Output::max - Output::min;

        const auto t = (static_cast<Input::underlying_type>(input) - Input::min) / static_cast<float>(input_range);
        return Output { Output::min + output_range * t };
    }

    export [[nodiscard]] constexpr analog_sample apply_dac(const digital_sample input)
    {
        return convert_sample<analog_sample>(input);
    }

    export class dac
    {
    public:
        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool new_enabled) { enabled = new_enabled; }

        [[nodiscard]] analog_sample convert(const digital_sample input) const
        {
            return enabled ? analog_sample { 1.0f * convert_sample<analog_sample>(input).data() } : analog_sample { };
        }

        [[nodiscard]] stereo<analog_sample> convert(const stereo<digital_sample> input) const
        {
            return { convert(input.left), convert(input.right) };
        }

    private:
        bool enabled { false };
    };

}