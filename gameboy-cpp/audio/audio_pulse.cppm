export module audio:pulse;

import std;
import :common;

namespace audio
{
    export struct sweep_unit
    {
        std::uint8_t pace{};
        sweep_direction direction{};
        std::uint8_t step{};

        bool enabled{};
        std::uint8_t timer{};
        std::uint16_t period_shadow{};

        void reset(const std::uint16_t current_period)
        {
            period_shadow = current_period;
            timer = pace;
            enabled = step != 0 || pace != 0;
        }

        [[nodiscard]] std::uint16_t compute_frequency() const
        {
            const bool should_negate = direction == sweep_direction::substraction;
            const auto shifted_period = period_shadow >> step;
            const auto direction_applied = (shifted_period ^ !should_negate) + should_negate;

            return direction_applied + period_shadow;
        }
    };

    export struct length_unit
    {
        bool enabled{};
        std::uint8_t initial_timer_value{};
        std::uint8_t current_timer_value{};

        [[nodiscard]] bool expired() const { return current_timer_value >= 64; }
        void reset() { current_timer_value = initial_timer_value; }
        void reset_if_expired() { if (expired()) { reset(); } }
        void tick() { current_timer_value++; }
    };

    export std::uint8_t get_pulse_waveform(const duty_cycle cycle)
    {
        static constexpr std::uint8_t waveform_0 = 0b00000001;
        static constexpr std::uint8_t waveform_1 = 0b10000001;
        static constexpr std::uint8_t waveform_2 = 0b10000111;
        static constexpr std::uint8_t waveform_3 = 0b01111110;

        using enum duty_cycle;

        switch (cycle)
        {
        case _12_5: return waveform_0;
        case _25: return waveform_1;
        case _50: return waveform_2;
        case _75: return waveform_3;
        default: std::unreachable();
        }
    }

    export std::uint8_t sample_pulse_waveform(const std::uint8_t sample, const duty_cycle cycle)
    {
        return get_pulse_waveform(cycle) >> sample & 0b1;
    }

    export struct envelope_unit
    {
        std::uint8_t initial_volume{};
        envelope_direction direction{};
        std::uint8_t pace{};

        std::uint8_t current_volume{};
        std::uint8_t timer{};

        bool enabled{};

        void reset()
        {
            current_volume = initial_volume;
            timer = pace;
            enabled = timer != 0;
        }

        void tick()
        {
            if (--timer == 0)
            {
                const auto gain = direction == envelope_direction::increase ? 1 : -1;

                current_volume += gain;
                enabled = current_volume > 0 && current_volume < 15 && pace != 0;
                timer = pace;
            }
        }
    };

    export struct period_unit
    {
        std::uint16_t initial_value{};
        std::uint16_t current_value{};

        void reset() { current_value = initial_value; }
    };

    export class pulse_channel
    {
    public:
        [[nodiscard]] bool active() const { return enabled; }
        [[nodiscard]] digital_sample output() const { return current_output; }

        [[nodiscard]] sweep_config get_sweep_config() const { return {}; }
        void set_sweep_config(const sweep_config config) {}

        [[nodiscard]] duty_cycle get_duty_cycle() const { return {}; }
        void set_duty_cycle(const duty_cycle) {}

        [[nodiscard]] bool is_length_timer_enabled() const { return false; }
        void set_length_timer_enabled(const bool enabled) {}

        [[nodiscard]] std::uint8_t get_length_timer() const { return {}; }
        void set_length_timer(const std::uint8_t value) {}

        [[nodiscard]] envelope_config get_envelope_config() const { return {}; }
        void set_envelope_config(const envelope_config envelope) {}

        [[nodiscard]] std::uint16_t get_period() const { return {}; }
        void set_period(const std::uint16_t period) {}

        void trigger()
        {
            length.reset_if_expired();
            period.reset();
            envelope.reset();
            sweep.reset(period.current_value);
            duty_timer = (2048 - period.current_value) * 4;

            if (sweep.step != 0)
            {
                const auto sweep_frequency = sweep.compute_frequency();
                const bool overflow = sweep_frequency > 0x7FFF;

                enabled = !overflow;
            }
        }

        void tick_envelope()
        {
            if (envelope.enabled)
            {
                envelope.tick();
            }
        }

        void tick_length_timer()
        {
            if (length.enabled)
            {
                length.tick();
                enabled = !length.expired();
            }
        }

        void tick_sweep()
        {
            if (sweep.enabled && sweep.pace != 0)
            {
                const auto sweep_frequency = sweep.compute_frequency();
                const bool overflow = sweep_frequency > 0x7FFF;

                if (!overflow && sweep.step != 0)
                {
                    sweep.period_shadow = sweep_frequency;
                    period.initial_value = sweep_frequency;

                    enabled = sweep.compute_frequency() <= 0x7FFF;
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
        bool enabled{true};
        digital_sample current_output{};

        sweep_unit sweep{};
        length_unit length{};
        duty_cycle duty_cycle{};
        envelope_unit envelope{};
        period_unit period{};

        std::uint8_t duty_step{};
        std::uint32_t duty_timer{};
    };
}
