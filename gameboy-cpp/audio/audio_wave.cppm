
export module audio:wave;

import std;
import :common;

namespace audio
{
    export class wave_channel
    {
    public:
        [[nodiscard]] bool active() const { return false; }
        [[nodiscard]] digital_sample output() const { return {}; }

        [[nodiscard]] bool is_length_timer_enabled() const { return false; }
        void set_length_timer_enabled(const bool value) { }

        [[nodiscard]] std::uint8_t get_length_timer() const { return {}; }
        void set_length_timer(const std::uint8_t value) { }

        [[nodiscard]] volume_level get_volume() const { return {}; }
        void set_volume(const volume_level value) { }

        [[nodiscard]] std::uint16_t get_period() const { return {}; }
        void set_period(const std::uint16_t value) { }

        void trigger() {}
        void tick_length_timer() { }
        void tick(std::uint32_t num_ticks) {};
    };

}