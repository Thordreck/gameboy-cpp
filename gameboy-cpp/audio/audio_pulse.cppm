export module audio:pulse;

import std;
import :common;
import :channels;

namespace audio
{
    export class pulse_channel
    {
    public:
        [[nodiscard]] bool active() const { return enabled; }
        void set_active(const bool active) { enabled = active; }

        [[nodiscard]] digital_sample output() const { return current_output; }

        [[nodiscard]] sweep_config get_sweep_config() const { return sweep.config; }
        void set_sweep_config(const sweep_config config)
        {
            using enum sweep_direction;
            const bool was_in_sub_mode = sweep.config.direction == substraction;

            sweep.config = config;

            if (was_in_sub_mode
                && config.direction != substraction
                && sweep.sweep_calculation_done_in_sub_mode)
            {
                enabled = false;
            }
        }

        [[nodiscard]] duty_cycle get_duty_cycle() const { return duty_cycle; }
        void set_duty_cycle(const duty_cycle duty) { duty_cycle = duty; }

        [[nodiscard]] bool is_length_timer_enabled() const { return length.enabled(); }
        void set_length_timer_enabled(const bool enabled) { length.set_enabled(enabled); }

        [[nodiscard]] std::uint8_t get_length_timer() const { return length.get_initial_value(); }
        void set_length_timer(const std::uint8_t value) { length.set_initial_value(value); }
        [[nodiscard]] bool is_length_timer_at_max_value() const { return length.is_at_max_value(); }

        [[nodiscard]] envelope_config get_envelope_config() const { return envelope.config; }
        void set_envelope_config(const envelope_config config) { envelope.config = config; }

        [[nodiscard]] std::uint16_t get_period() const { return period.initial_value; }
        void set_period(const std::uint16_t value) { period.initial_value = value; }

        void trigger()
        {
            enabled = true;
            length.trigger();
            period.reset();
            envelope.reset();
            sweep.reset(period.current_value);
            duty_timer = (2048 - period.current_value) * 4;

            if (sweep.config.step != 0)
            {
                const auto sweep_frequency = sweep.compute_frequency();
                const bool overflow = sweep_frequency > 0x7FF;

                enabled = !overflow;
            }
        }

        void tick_envelope()
        {
            envelope.tick();
        }

        void tick_length_timer()
        {
            if (length.enabled())
            {
                length.tick();

                if (length.expired())
                {
                    enabled = false;
                }
            }
        }

        void tick_sweep()
        {
            if (sweep.tick() && sweep.enabled && sweep.config.pace != 0)
            {
                const auto sweep_frequency = sweep.compute_frequency();
                const bool overflow = sweep_frequency > 0x7FF;

                if (overflow)
                {
                    enabled = false;
                }

                if (!overflow && sweep.config.step != 0)
                {
                    sweep.period_shadow = sweep_frequency;
                    period.initial_value = sweep_frequency;

                    if (sweep.compute_frequency() > 0x7FF)
                    {
                        enabled = false;
                    }
                }
            }
        }

        void tick(std::uint32_t num_ticks)
        {
            while (num_ticks > 0)
            {
                const std::uint32_t consumed_ticks = std::min(num_ticks, duty_timer);
                num_ticks -= consumed_ticks;
                duty_timer -= consumed_ticks;

                if (duty_timer == 0)
                {
                    const std::uint8_t sampled_waveform = sample_pulse_waveform(duty_step, duty_cycle);
                    current_output = sampled_waveform * envelope.current_volume;

                    duty_timer = (2048 - period.current_value) * 4;
                    duty_step = (duty_step + 1) & 7;
                }
            }
        }

    private:
        bool enabled{false};
        digital_sample current_output{};

        sweep_unit sweep{};
        length_unit_64 length{};
        duty_cycle duty_cycle{};
        envelope_unit envelope{};
        period_unit period{};

        std::uint8_t duty_step{};
        std::uint32_t duty_timer{};
    };
}
