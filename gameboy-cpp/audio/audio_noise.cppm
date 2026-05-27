
export module audio:noise;

import std;
import utilities;

import :common;
import :channels;

namespace audio
{
    export class noise_channel
    {
    public:
        [[nodiscard]] bool active() const { return enabled; }
        [[nodiscard]] digital_sample output() const { return current_output; }

        [[nodiscard]] bool is_length_timer_enabled() const { return length.enabled; }
        void set_length_timer_enabled(const bool value) { length.enabled = value; }

        [[nodiscard]] std::uint8_t get_length_timer() const { return length.initial_timer_value; }
        void set_length_timer(const std::uint8_t value) { length.initial_timer_value = value;}

        [[nodiscard]] envelope_config get_envelope_config() const { return envelope.config; }
        void set_envelope_config(const envelope_config value) { envelope.config = value; }

        [[nodiscard]] randomness_config get_randomness_config() const { return randomness; }
        void set_randomness_config(const randomness_config value) { randomness = value; }

        void trigger()
        {
            enabled = true;

            length.reset_if_expired();
            envelope.reset();
            lfsr = {};
            lfsr_timer = compute_noise_period(randomness);
        }

        void tick_length_timer()
        {
            if (length.enabled)
            {
                length.tick();
                enabled = !length.expired();
            }
        }

        void tick_envelope()
        {
            if (envelope.enabled)
            {
                envelope.tick();
            }
        }

        void tick(std::uint32_t num_ticks)
        {
            while (num_ticks > 0)
            {
                const std::uint32_t consumed_ticks = std::min(num_ticks, lfsr_timer);

                lfsr_timer -= consumed_ticks;
                num_ticks -= consumed_ticks;

                if (lfsr_timer == 0)
                {
                    using namespace utils;
                    const std::uint8_t new_value = (lfsr & 0b1) ^ (lfsr >> 1 & 0b1) & 0b1;

                    utils::write_bit<15>(lfsr, new_value);

                    if (randomness.lfsr_width == lfsr_width::_7_bit)
                    {
                        utils::write_bit<7>(lfsr, new_value);
                    }

                    lfsr >>= 1;
                    current_output = (lfsr & 0b1) ? envelope.current_volume : 0;
                    lfsr_timer = compute_noise_period(randomness);
                }
            }
        };

    private:
        bool enabled { false };
        digital_sample current_output{};

        length_unit length {};
        envelope_unit envelope {};
        randomness_config randomness {};

        std::uint16_t lfsr {};
        std::uint32_t lfsr_timer {};
    };
}