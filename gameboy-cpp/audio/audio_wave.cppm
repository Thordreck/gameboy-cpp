
export module audio:wave;

import std;
import :common;
import :channels;

namespace audio
{
    export class wave_channel
    {
    public:
        [[nodiscard]] bool active() const { return enabled; }
        [[nodiscard]] digital_sample output() const { return current_output; }

        [[nodiscard]] bool is_length_timer_enabled() const { return length.enabled; }
        void set_length_timer_enabled(const bool value) { length.enabled = value; }

        [[nodiscard]] std::uint8_t get_length_timer() const { return length.initial_timer_value; }
        void set_length_timer(const std::uint8_t value) { length.initial_timer_value = value; }

        [[nodiscard]] volume_level get_volume() const { return volume.initial_value; }
        void set_volume(const volume_level value) { volume.initial_value = value; }

        [[nodiscard]] std::uint16_t get_period() const { return period.initial_value; }
        void set_period(const std::uint16_t value) { period.initial_value = value; }

        void trigger()
        {
            enabled = true;
            length.reset_if_expired();
            period.reset();
            volume.reset();
            wave_ram_index = 0;
            wave_timer = (2048 - period.current_value) * 2;
        }

        void tick_length_timer()
        {
            if (length.enabled)
            {
                length.tick();
                enabled = !length.expired();
            }
        }

        void tick(std::uint32_t num_ticks, const_wave_ram_view_t wave_ram)
        {
            while (num_ticks > 0)
            {
                const std::uint32_t consumed_ticks = std::min(num_ticks, wave_timer);

                wave_timer -= consumed_ticks;
                num_ticks -= consumed_ticks;

                if (wave_timer == 0)
                {
                    wave_ram_index = (wave_ram_index + 1) & 31;
                    wave_timer = (2048 - period.current_value) * 2;

                    const std::uint32_t byte_index = wave_ram_index / 2;
                    const std::uint8_t nibble_index = wave_ram_index % 2;

                    const std::uint8_t sample_byte = wave_ram[byte_index];
                    const std::uint8_t sample_nibble = (sample_byte >> 4 * (1 - nibble_index)) & 0xF;

                    current_output = apply_volume(sample_nibble, volume.current_value);
                }
            }
        };

    private:
        bool enabled { false };
        digital_sample current_output{};

        length_unit length {};
        period_unit period {};
        volume_unit volume {};

        std::uint8_t wave_ram_index {};
        std::uint32_t wave_timer {};
    };

}