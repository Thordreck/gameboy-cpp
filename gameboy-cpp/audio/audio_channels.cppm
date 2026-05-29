export module audio:channels;

import std;
import :common;

namespace audio
{
    export struct sweep_unit
    {
        sweep_config config{};

        bool enabled{};
        std::uint8_t timer{};
        std::uint16_t period_shadow{};

        void reset(const std::uint16_t current_period)
        {
            period_shadow = current_period;
            timer = config.pace;
            enabled = config.step != 0 || config.pace != 0;
        }

        [[nodiscard]] std::uint16_t compute_frequency() const
        {
            const bool should_negate = config.direction == sweep_direction::substraction;
            const auto shifted_period = period_shadow >> config.step;
            const auto direction_applied = (shifted_period ^ !should_negate) + should_negate;

            return direction_applied + period_shadow;
        }
    };

    export template<std::uint16_t TimerMax>
    struct length_unit
    {
        [[nodiscard]] bool enabled() const { return is_enabled; }
        void set_enabled(const bool enabled) { is_enabled = enabled; }

        [[nodiscard]] bool expired() const { return current_value == 0; }
        [[nodiscard]] bool is_at_max_value() const { return current_value == TimerMax; }

        [[nodiscard]] std::uint8_t get_initial_value() const { return initial_value; }
        void set_initial_value(const std::uint8_t value)
        {
            initial_value = value;
            current_value = TimerMax - initial_value;
        }

        void trigger()
        {
            if (expired())
            {
                current_value = TimerMax;
            }
        }

        void tick()
        {
            if (current_value > 0)
            {
                current_value--;
            }
        }

    private:
        bool is_enabled { false };

        std::uint8_t initial_value{};
        std::uint16_t current_value{};
    };

    export using length_unit_64 = length_unit<64>;
    export using length_unit_256 = length_unit<256>;

    export std::uint8_t get_pulse_waveform(const duty_cycle cycle)
    {
        static constexpr std::uint8_t waveform_0 = 0b00000001;
        static constexpr std::uint8_t waveform_1 = 0b00000011;
        static constexpr std::uint8_t waveform_2 = 0b00001111;
        static constexpr std::uint8_t waveform_3 = 0b11111100;

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
        envelope_config config{};
        std::uint8_t current_volume{};
        std::uint8_t timer{};

        void reset()
        {
            current_volume = config.volume;
            timer = config.pace;
        }

        void tick()
        {
            if (config.pace == 0)
            {
                return;
            }

            if (--timer == 0)
            {
                using enum envelope_direction;

                if ((current_volume < 0xF && config.direction == increase) || (current_volume > 0x0 && config.direction == decrease))
                {
                    const auto gain = config.direction == increase ? 1 : -1;
                    current_volume += gain;
                }

                timer = config.pace;
            }
        }
    };

    export struct period_unit
    {
        std::uint16_t initial_value{};
        std::uint16_t current_value{};

        void reset() { current_value = initial_value; }
    };

    export struct volume_unit
    {
        volume_level initial_value{};
        volume_level current_value{};

        void reset() { current_value = initial_value; }
    };

    export [[nodiscard]] std::uint8_t apply_volume(const std::uint8_t input, const volume_level volume)
    {
        using enum volume_level;

        switch (volume)
        {
        case mute: return {};
        case full: return input;
        case half: return input >> 1;
        case quarter: return input >> 2;
        default: std::unreachable();
        }
    }

    export [[nodiscard]] std::uint32_t compute_noise_period(const randomness_config& config)
    {
        constexpr std::array<std::uint8_t, 8> divisors { 8, 16, 32, 48, 64, 80, 96, 112 };
        return divisors[config.clock_divider] << config.clock_shift;
    }
}
