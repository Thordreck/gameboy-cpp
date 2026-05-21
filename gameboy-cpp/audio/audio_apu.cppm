
export module audio:apu;

import std;
import timer;

import :common;
import :div_apu;
import :pulse;

namespace audio
{
    export class audio_processing_unit
    {
    public:
        [[nodiscard]] analog_sample output() const { return current_output; }

        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool value) { enabled = value; }

        [[nodiscard]] bool active() const { return is_enabled(); }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            // TODO: implement properly
            return 4;
        }

        void tick(const std::uint32_t ticks, const timer::div div)
        {
            // TODO: update div_apu based on div
            // TODO: tick channels
            // TODO: dac
            // TODO: mix
            // TODO: hpf
        }

    private:
        bool enabled { true };
        analog_sample current_output {};

        div_apu div_apu {};
        pulse_channel channel_1 {};
    };

}