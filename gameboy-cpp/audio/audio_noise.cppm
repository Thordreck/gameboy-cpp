
export module audio:noise;

import std;
import :common;

namespace audio
{
    export class noise_channel
    {
    public:
        [[nodiscard]] bool active() const { return false; }
        [[nodiscard]] digital_sample output() const { return {}; }

        [[nodiscard]] bool is_length_timer_enabled() const { return false; }
        void set_length_timer_enabled(const bool value) { }

        [[nodiscard]] std::uint8_t get_length_timer() const { return {}; }
        void set_length_timer(const std::uint8_t value) { }

        [[nodiscard]] envelope_config get_envelope_config() const { return {}; }
        void set_envelope_config(const envelope_config value) { }

        [[nodiscard]] randomness_config get_randomness_config() const { return {}; }
        void set_randomness_config(const randomness_config value) { }

        void trigger() {}
        void tick_length_timer() { }
        void tick_envelope() {}
        void tick(std::uint32_t num_ticks) {};

    private:

    };
}