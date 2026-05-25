
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
        // Global
        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool value) { enabled = value; }

        [[nodiscard]] stereo_volume get_master_volume() const { return {}; }
        void set_master_volume(const stereo_volume) {}

        // Channel 1
        void trigger_channel_1() {}
        [[nodiscard]] bool channel_1_on() const { return false; }

        [[nodiscard]] stereo_panning get_channel_1_panning() const { return {}; }
        void set_channel_1_panning(const stereo_panning) {}

        [[nodiscard]] sweep_config get_channel_1_sweep() const { return {}; }
        void set_channel_1_sweep(const sweep_config) {}

        [[nodiscard]] duty_cycle get_channel_1_duty_cycle() const { return {}; }
        void set_channel_1_duty_cycle(const duty_cycle) {}

        [[nodiscard]] bool is_channel_1_length_timer_enabled() const { return false; }
        void enable_channel_1_length_timer(const bool value) { }

        [[nodiscard]] std::uint8_t get_channel_1_length_timer() const { return {}; }
        void set_channel_1_length_timer(const std::uint8_t) {}

        [[nodiscard]] envelope_config get_channel_1_envelope() const { return {}; }
        void set_channel_1_envelope(const envelope_config) {}

        [[nodiscard]] std::uint16_t get_channel_1_period() const { return {}; }
        void set_channel_1_period(const std::uint16_t) {}

        // Channel 2
        void trigger_channel_2() {}
        [[nodiscard]] bool channel_2_on() const { return false; }

        [[nodiscard]] stereo_panning get_channel_2_panning() const { return {}; }
        void set_channel_2_panning(const stereo_panning) {}

        [[nodiscard]] duty_cycle get_channel_2_duty_cycle() const { return {}; }
        void set_channel_2_duty_cycle(const duty_cycle) {}

        [[nodiscard]] bool is_channel_2_length_timer_enabled() const { return false; }
        void enable_channel_2_length_timer(const bool value) { }

        [[nodiscard]] std::uint8_t get_channel_2_length_timer() const { return {}; }
        void set_channel_2_length_timer(const std::uint8_t) {}

        [[nodiscard]] envelope_config get_channel_2_envelope() const { return {}; }
        void set_channel_2_envelope(const envelope_config) {}

        [[nodiscard]] std::uint16_t get_channel_2_period() const { return {}; }
        void set_channel_2_period(const std::uint16_t) {}

        // Channel 3
        void trigger_channel_3() {}
        [[nodiscard]] bool channel_3_on() const { return false; }

        [[nodiscard]] stereo_panning get_channel_3_panning() const { return {}; }
        void set_channel_3_panning(const stereo_panning) {}

        [[nodiscard]] bool is_channel_3_dac_enabled() const { return false; }
        void enable_channel_3_dac(const bool value) { }

        [[nodiscard]] bool is_channel_3_length_timer_enabled() const { return false; }
        void enable_channel_3_length_timer(const bool value) { }

        [[nodiscard]] std::uint8_t get_channel_3_length_timer() const { return {}; }
        void set_channel_3_length_timer(const std::uint8_t) {}

        [[nodiscard]] volume_level get_channel_3_output_level() const { return {}; }
        void set_channel_3_output_level(const volume_level) {}

        [[nodiscard]] std::uint16_t get_channel_3_period() const { return {}; }
        void set_channel_3_period(const std::uint16_t) {}

        // Channel 4
        void trigger_channel_4() {}
        [[nodiscard]] bool channel_4_on() const { return false; }

        [[nodiscard]] stereo_panning get_channel_4_panning() const { return {}; }
        void set_channel_4_panning(const stereo_panning) {}

        [[nodiscard]] bool is_channel_4_length_timer_enabled() const { return false; }
        void enable_channel_4_length_timer(const bool value) { }

        [[nodiscard]] std::uint8_t get_channel_4_length_timer() const { return {}; }
        void set_channel_4_length_timer(const std::uint8_t) {}

        [[nodiscard]] envelope_config get_channel_4_envelope() const { return {}; }
        void set_channel_4_envelope(const envelope_config) {}

        [[nodiscard]] randomness_config get_channel_4_randomness() const { return {}; }
        void set_channel_4_randomness(const randomness_config) {}

        // Others
        [[nodiscard]] bool active() const { return is_enabled(); }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            // TODO: implement properly
            return 4;
        }

        template<AudioSink<float> Sink>
        void tick(const std::uint32_t ticks, const timer::div div, Sink& sink)
        {
            // TODO: update div_apu based on div
            // TODO: tick channels
            // TODO: dac
            // TODO: mix
            // TODO: hpf
        }

    private:
        bool enabled { true };
        div_apu div_apu {};
        pulse_channel channel_1 {};
    };

}