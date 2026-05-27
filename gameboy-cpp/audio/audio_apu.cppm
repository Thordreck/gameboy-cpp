module;
#include "profiling.hpp"

export module audio:apu;

import std;
import timer;

import :dac;
import :hpf;
import :wave;
import :mixer;
import :noise;
import :pulse;
import :common;
import :div_apu;
import :panning;
import :amplifier;
import :normalization;

namespace audio
{
    export class audio_processing_unit
    {
    public:
        // Global
        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool value) { enabled = value; }

        [[nodiscard]] stereo_volume get_master_volume() const { return master_volume; }
        void set_master_volume(const stereo_volume volume) { master_volume = volume; }

        // Channel 1
        void trigger_channel_1() { channel_1.trigger(); }
        [[nodiscard]] bool channel_1_on() const { return channel_1.active(); }

        [[nodiscard]] stereo_panning get_channel_1_panning() const { return channels_panning[0].get_config(); }
        void set_channel_1_panning(const stereo_panning panning) { channels_panning[0].set_config(panning); }

        [[nodiscard]] sweep_config get_channel_1_sweep() const { return channel_1.get_sweep_config(); }
        void set_channel_1_sweep(const sweep_config config) { channel_1.set_sweep_config(config); }

        [[nodiscard]] duty_cycle get_channel_1_duty_cycle() const { return channel_1.get_duty_cycle(); }
        void set_channel_1_duty_cycle(const duty_cycle duty) { channel_1.set_duty_cycle(duty); }

        [[nodiscard]] bool is_channel_1_length_timer_enabled() const { return channel_1.is_length_timer_enabled(); }
        void enable_channel_1_length_timer(const bool value) { channel_1.set_length_timer_enabled(value); }

        [[nodiscard]] std::uint8_t get_channel_1_length_timer() const { return channel_1.get_length_timer(); }
        void set_channel_1_length_timer(const std::uint8_t value) { channel_1.set_length_timer(value); }

        [[nodiscard]] envelope_config get_channel_1_envelope() const { return channel_1.get_envelope_config(); }
        void set_channel_1_envelope(const envelope_config config) { channel_1.set_envelope_config(config); }

        [[nodiscard]] std::uint16_t get_channel_1_period() const { return channel_1.get_period(); }
        void set_channel_1_period(const std::uint16_t value) { channel_1.set_period(value); }

        // Channel 2
        void trigger_channel_2() { channel_2.trigger(); }
        [[nodiscard]] bool channel_2_on() const { return channel_2.active(); }

        [[nodiscard]] stereo_panning get_channel_2_panning() const { return channels_panning[1].get_config(); }
        void set_channel_2_panning(const stereo_panning panning) { channels_panning[1].set_config(panning); }

        [[nodiscard]] duty_cycle get_channel_2_duty_cycle() const { return channel_2.get_duty_cycle(); }
        void set_channel_2_duty_cycle(const duty_cycle duty) { channel_2.set_duty_cycle(duty); }

        [[nodiscard]] bool is_channel_2_length_timer_enabled() const { return channel_2.is_length_timer_enabled(); }
        void enable_channel_2_length_timer(const bool value) { channel_2.set_length_timer_enabled(value); }

        [[nodiscard]] std::uint8_t get_channel_2_length_timer() const { return channel_2.get_length_timer(); }
        void set_channel_2_length_timer(const std::uint8_t value) { channel_2.set_length_timer(value); }

        [[nodiscard]] envelope_config get_channel_2_envelope() const { return channel_2.get_envelope_config(); }
        void set_channel_2_envelope(const envelope_config config) { channel_2.set_envelope_config(config); }

        [[nodiscard]] std::uint16_t get_channel_2_period() const { return channel_2.get_period(); }
        void set_channel_2_period(const std::uint16_t value) { channel_2.set_period(value); }

        // Channel 3
        void trigger_channel_3() { channel_3.trigger(); }
        [[nodiscard]] bool channel_3_on() const { return channel_3.active(); }

        [[nodiscard]] stereo_panning get_channel_3_panning() const { return channels_panning[2].get_config(); }
        void set_channel_3_panning(const stereo_panning panning) { channels_panning[2].set_config(panning); }

        [[nodiscard]] bool is_channel_3_dac_enabled() const { return channels_dac[2].is_enabled(); }
        void enable_channel_3_dac(const bool value) { channels_dac[2].set_enabled(value); }

        [[nodiscard]] bool is_channel_3_length_timer_enabled() const { return channel_3.is_length_timer_enabled(); }
        void enable_channel_3_length_timer(const bool value) { channel_3.set_length_timer_enabled(value); }

        [[nodiscard]] std::uint8_t get_channel_3_length_timer() const { return channel_3.get_length_timer(); }
        void set_channel_3_length_timer(const std::uint8_t value) { channel_3.set_length_timer(value); }

        [[nodiscard]] volume_level get_channel_3_output_level() const { return channel_3.get_volume(); }
        void set_channel_3_output_level(const volume_level value) { channel_3.set_volume(value); }

        [[nodiscard]] std::uint16_t get_channel_3_period() const { return channel_3.get_period(); }
        void set_channel_3_period(const std::uint16_t value) { channel_3.set_period(value); }

        // Channel 4
        void trigger_channel_4() { channel_4.trigger(); }
        [[nodiscard]] bool channel_4_on() const { return channel_4.active(); }

        [[nodiscard]] stereo_panning get_channel_4_panning() const { return channels_panning[3].get_config(); }
        void set_channel_4_panning(const stereo_panning panning) { channels_panning[3].set_config(panning); }

        [[nodiscard]] bool is_channel_4_length_timer_enabled() const { return channel_4.is_length_timer_enabled(); }
        void enable_channel_4_length_timer(const bool value) { channel_4.set_length_timer_enabled(value); }

        [[nodiscard]] std::uint8_t get_channel_4_length_timer() const { return channel_4.get_length_timer(); }
        void set_channel_4_length_timer(const std::uint8_t value) { channel_4.set_length_timer(value); }

        [[nodiscard]] envelope_config get_channel_4_envelope() const { return channel_4.get_envelope_config(); }
        void set_channel_4_envelope(const envelope_config config) { channel_4.set_envelope_config(config); }

        [[nodiscard]] randomness_config get_channel_4_randomness() const { return channel_4.get_randomness_config(); }
        void set_channel_4_randomness(const randomness_config config) { channel_4.set_randomness_config(config); }

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
            PROFILER_SCOPE("APU::tick()");

            tick_frame_sequencer(div);
            tick_channels(ticks, sink);
        }

    private:
        void tick_frame_sequencer(const timer::div current_div)
        {
            if (!div_apu.tick(current_div)) { return; }

            switch (div_apu.value())
            {
            case 2:
            case 6:
                tick_length_timer();
                break;
            case 4:
                tick_length_timer();
                tick_ch1_sweep();
                break;
            case 8:
                tick_length_timer();
                tick_envelope_sweep();
                break;
            default:
                break;
            }
        }

        void tick_length_timer()
        {
            channel_1.tick_length_timer();
            channel_2.tick_length_timer();
            channel_3.tick_length_timer();
            channel_4.tick_length_timer();
        }

        void tick_ch1_sweep()
        {
            channel_1.tick_sweep();
        }

        void tick_envelope_sweep()
        {
            channel_1.tick_envelope();
            channel_2.tick_envelope();
            channel_4.tick_envelope();
        }

        template<AudioSink<float> Sink>
        void tick_channels(std::uint32_t num_ticks, Sink& sink)
        {
            const std::uint32_t output_sample_rate = sink.sample_rate();

            while (num_ticks > 0)
            {
                constexpr std::uint32_t master_clock_rate { 4194304 };

                const std::uint32_t ticks_until_next_sample =
                    (master_clock_rate - tick_accumulator + output_sample_rate - 1) / output_sample_rate;
                const std::uint32_t ticks_to_consume = std::min(ticks_until_next_sample, num_ticks);

                channel_1.tick(ticks_to_consume);
                channel_2.tick(ticks_to_consume);
                channel_3.tick(ticks_to_consume);
                channel_4.tick(ticks_to_consume);

                num_ticks -= ticks_to_consume;
                tick_accumulator += ticks_to_consume * output_sample_rate;

                if (tick_accumulator >= master_clock_rate)
                {
                    tick_accumulator -= master_clock_rate;

                    const auto [left, right] = generate_sample();
                    const std::array raw_samples { left.data(), right.data() };

                    sink.write(raw_samples);
                }
            }
        }

        stereo<analog_sample> generate_sample()
        {
            // Channels
            const digital_sample digital_ch1 = channel_1.output();
            const digital_sample digital_ch2 = channel_2.output();
            const digital_sample digital_ch3 = channel_3.output();
            const digital_sample digital_ch4 = channel_4.output();

            // DAC
            const analog_sample analog_ch1 = channels_dac[0].convert(digital_ch1);
            const analog_sample analog_ch2 = channels_dac[1].convert(digital_ch2);
            const analog_sample analog_ch3 = channels_dac[2].convert(digital_ch3);
            const analog_sample analog_ch4 = channels_dac[3].convert(digital_ch4);

            // Stereo
            const stereo<analog_sample> stereo_analog_ch1 = channels_panning[0].apply(analog_ch1);
            const stereo<analog_sample> stereo_analog_ch2 = channels_panning[1].apply(analog_ch2);
            const stereo<analog_sample> stereo_analog_ch3 = channels_panning[2].apply(analog_ch3);
            const stereo<analog_sample> stereo_analog_ch4 = channels_panning[3].apply(analog_ch4);

            // Mix, normalization, gain, filter
            const auto mixed = mix(stereo_analog_ch1, stereo_analog_ch2, stereo_analog_ch3, stereo_analog_ch4);
            const auto normalized = normalize<num_apu_channels>(mixed);

            const stereo master_gain { (1 + master_volume.left) / 8.0f, 1 + master_volume.right / 8.0f };
            const auto amplified = apply_gain(normalized, master_gain);

            return high_pass_filter.apply(amplified);
        }

        bool enabled { true };
        std::uint64_t tick_accumulator {};

        div_apu div_apu {};
        stereo_volume master_volume {};

        pulse_channel channel_1 {};
        pulse_channel channel_2 {};
        wave_channel channel_3 {};
        noise_channel channel_4 {};

        channel_array_t<panning> channels_panning {};
        channel_array_t<dac> channels_dac {};
        stereo_high_pass_filter high_pass_filter {};
    };

}