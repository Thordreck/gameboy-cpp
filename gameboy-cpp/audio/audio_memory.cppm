
export module audio:memory;

import :apu;
import memory;
import utilities;

namespace audio
{
    export constexpr memory::memory_address_t channel_1_sweep_address = 0xFF10;
    export constexpr memory::memory_address_t channel_1_length_and_cycle_address = 0xFF11;
    export constexpr memory::memory_address_t channel_1_envelope_address = 0xFF12;
    export constexpr memory::memory_address_t channel_1_period_low_address = 0xFF13;
    export constexpr memory::memory_address_t channel_1_period_high_and_control_address = 0xFF14;

    export constexpr memory::memory_address_t channel_2_length_and_cycle_address = 0xFF16;
    export constexpr memory::memory_address_t channel_2_envelope_address = 0xFF17;
    export constexpr memory::memory_address_t channel_2_period_low_address = 0xFF18;
    export constexpr memory::memory_address_t channel_2_period_high_and_control_address = 0xFF19;

    export constexpr memory::memory_address_t channel_3_dac_enable_address = 0xFF1A;
    export constexpr memory::memory_address_t channel_3_length_timer_address = 0xFF1B;
    export constexpr memory::memory_address_t channel_3_output_level_address = 0xFF1C;
    export constexpr memory::memory_address_t channel_3_period_low_address = 0xFF1D;
    export constexpr memory::memory_address_t channel_3_period_high_and_control_address = 0xFF1E;

    export constexpr memory::memory_address_t channel_4_length_timer_address = 0xFF20;
    export constexpr memory::memory_address_t channel_4_volume_and_envelop_address = 0xFF21;
    export constexpr memory::memory_address_t channel_4_frequency_and_randomness_address = 0xFF22;
    export constexpr memory::memory_address_t channel_4_control_address = 0xFF23;

    export constexpr memory::memory_address_t master_volume_and_vin_panning_address = 0xFF24;
    export constexpr memory::memory_address_t sound_panning_address = 0xFF25;
    export constexpr memory::memory_address_t master_control_address = 0xFF26;

    export constexpr memory::memory_address_t wave_ram_start_address = 0xFF30;
    export constexpr memory::memory_address_t wave_ram_end_address = 0xFF3F;

    // Global - Read
    export [[nodiscard]] memory::memory_data_t read_master_control_address(const audio_processing_unit& apu)
    {
        return (apu.is_enabled() << 7)
            | (apu.channel_4_on() << 3)
            | (apu.channel_3_on() << 2)
            | (apu.channel_2_on() << 1)
            | apu.channel_1_on();
    }

    export [[nodiscard]] memory::memory_data_t read_sound_panning_address(const audio_processing_unit& apu)
    {
        const auto [ch1_left, ch1_right] = apu.get_channel_1_panning();
        const auto [ch2_left, ch2_right] = apu.get_channel_2_panning();
        const auto [ch3_left, ch3_right] = apu.get_channel_3_panning();
        const auto [ch4_left, ch4_right] = apu.get_channel_4_panning();

        return (ch4_left << 7) | (ch4_right << 3)
            | (ch3_left << 6) | (ch3_right << 2)
            | (ch2_left << 5) | (ch2_right << 1)
            | (ch1_left << 4) | (ch1_right << 0);
    }

    export [[nodiscard]] memory::memory_data_t read_master_volume_address(const audio_processing_unit& apu)
    {
        const auto [master_left, master_right] = apu.get_master_volume();
        return (master_left & 0b111 << 6) | (master_right & 0b111);
    }

    // Channel 1 - Read
    export [[nodiscard]] memory::memory_data_t read_channel_1_sweep_address(const audio_processing_unit& apu)
    {
        const auto [pace, direction, step] = apu.get_channel_1_sweep();
        return ((pace & 0b111) << 6) | ((std::to_underlying(direction) & 0b1)<< 3) | (step & 0b111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_1_length_and_cycle_address(const audio_processing_unit& apu)
    {
        return ((std::to_underlying(apu.get_channel_1_duty_cycle()) & 0b11) << 7) | (apu.get_channel_1_length_timer() & 0b111111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_1_envelope_address(const audio_processing_unit& apu)
    {
        const auto [volume, direction, pace] = apu.get_channel_1_envelope();
        return ((volume & 0b1111) << 7) | ((std::to_underlying(direction) & 0b1) << 3) | (pace & 0b111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_1_period_low_address(const audio_processing_unit& apu)
    {
        // Write-only
        return 0xFF;
    }

    export [[nodiscard]] memory::memory_data_t read_channel_1_period_high_and_control_address(const audio_processing_unit& apu)
    {
        return apu.is_channel_1_length_timer_enabled() << 6;
    }

    // Channel 2 - Read
    export [[nodiscard]] memory::memory_data_t read_channel_2_length_and_cycle_address(const audio_processing_unit& apu)
    {
        return ((std::to_underlying(apu.get_channel_2_duty_cycle()) & 0b11) << 7) | (apu.get_channel_2_length_timer() & 0b111111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_2_envelope_address(const audio_processing_unit& apu)
    {
        const auto [volume, direction, pace] = apu.get_channel_2_envelope();
        return ((volume & 0b1111) << 7) | ((std::to_underlying(direction) & 0b1) << 3) | (pace & 0b111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_2_period_low_address(const audio_processing_unit& apu)
    {
        // Write-only
        return 0xFF;
    }

    export [[nodiscard]] memory::memory_data_t read_channel_2_period_high_and_control_address(const audio_processing_unit& apu)
    {
        return apu.is_channel_2_length_timer_enabled() << 6;
    }

    // Channel 3 - Read
    export [[nodiscard]] memory::memory_data_t read_channel_3_dac_enable_address(const audio_processing_unit& apu)
    {
        return apu.is_channel_3_dac_enabled() << 7;
    }

    export [[nodiscard]] memory::memory_data_t read_channel_3_length_timer_address(const audio_processing_unit& apu)
    {
        return apu.get_channel_3_length_timer();
    }

    export [[nodiscard]] memory::memory_data_t read_channel_3_output_level_address(const audio_processing_unit& apu)
    {
        return (std::to_underlying(apu.get_channel_3_output_level()) & 0b11) << 6;
    }

    export [[nodiscard]] memory::memory_data_t read_channel_3_period_low_address(const audio_processing_unit& apu)
    {
        // Write-only
        return 0xFF;
    }

    export [[nodiscard]] memory::memory_data_t read_channel_3_period_high_and_control_address(const audio_processing_unit& apu)
    {
        return apu.is_channel_3_length_timer_enabled() << 6;
    }

    // Channel 4 - Read
    export [[nodiscard]] memory::memory_data_t read_channel_4_length_timer_address(const audio_processing_unit& apu)
    {
        return apu.get_channel_4_length_timer() & 0b11111;
    }

    export [[nodiscard]] memory::memory_data_t read_channel_4_envelope_address(const audio_processing_unit& apu)
    {
        const auto [volume, direction, pace] = apu.get_channel_4_envelope();
        return ((volume & 0b1111) << 7) | ((std::to_underlying(direction) & 0b1) << 3) | (pace & 0b111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_4_frequency_and_randomness_address(const audio_processing_unit& apu)
    {
        const auto [clock_shift, lfsr_width, clock_divider] = apu.get_channel_4_randomness();
        return ((clock_shift & 0b1111) << 7) | ((std::to_underlying(lfsr_width) & 0b1) << 3) | (clock_divider & 0b111);
    }

    export [[nodiscard]] memory::memory_data_t read_channel_4_control_address(const audio_processing_unit& apu)
    {
        return apu.is_channel_4_length_timer_enabled() << 6;
    }

    // Global - Write
    export void write_master_control_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const bool apu_enabled = (value >> 7) & 0b1;
        apu.set_enabled(apu_enabled);
    }

    export void write_sound_panning_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        using namespace utils;

        const stereo_panning ch4_panning { is_bit_set<7>(value), is_bit_set<3>(value) };
        const stereo_panning ch3_panning { is_bit_set<6>(value), is_bit_set<2>(value) };
        const stereo_panning ch2_panning { is_bit_set<5>(value), is_bit_set<1>(value) };
        const stereo_panning ch1_panning { is_bit_set<4>(value), is_bit_set<0>(value) };

        apu.set_channel_4_panning(ch4_panning);
        apu.set_channel_3_panning(ch3_panning);
        apu.set_channel_2_panning(ch2_panning);
        apu.set_channel_1_panning(ch1_panning);
    }

    export void write_master_volume_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const stereo_volume master_volume
        {
            static_cast<std::uint8_t>((value >> 4) & 0b111),
            static_cast<std::uint8_t>(value & 0b111)
        };

        apu.set_master_volume(master_volume);
    }

    // Channel 1 - Write
    export void write_channel_1_sweep_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const sweep_config sweep
        {
            static_cast<std::uint8_t>((value >> 4) & 0b111),
            static_cast<sweep_direction>((value >> 3) & 0b1),
            static_cast<std::uint8_t>(value & 0b111)
        };

        apu.set_channel_1_sweep(sweep);
    }

    export void write_channel_1_length_and_cycle_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const auto duty = static_cast<duty_cycle>((value >> 6) & 0b11);
        const std::uint8_t length_timer = value & 0b111111;

        apu.set_channel_1_duty_cycle(duty);
        apu.set_channel_1_length_timer(length_timer);
    }

    export void write_channel_1_envelope_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const envelope_config envelope
        {
            static_cast<std::uint8_t>((value >> 4) & 0b1111),
            static_cast<envelope_direction>((value >> 3) & 0b1),
            static_cast<std::uint8_t>(value & 0b111)
        };

        apu.set_channel_1_envelope(envelope);
    }

    export void write_channel_1_period_low_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const std::uint16_t prev_period = apu.get_channel_1_period();
        const std::uint16_t new_period = (prev_period & (0b111 << 8)) | value;

        apu.set_channel_1_period(new_period);
    }

    export void write_channel_1_period_high_and_control_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const bool should_trigger = utils::is_bit_set<7>(value);
        const bool enable_length_timer = utils::is_bit_set<6>(value);
        const std::uint8_t period_high = value & 0b111;

        const std::uint16_t prev_period = apu.get_channel_1_period();
        const std::uint16_t new_period = (prev_period & 0xFF) | (period_high << 8);

        apu.set_channel_1_period(new_period);
        apu.enable_channel_1_length_timer(enable_length_timer);

        if (should_trigger)
        {
            apu.trigger_channel_1();
        }
    }

    // Channel 2 - Write
    export void write_channel_2_length_and_cycle_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const auto duty = static_cast<duty_cycle>((value >> 6) & 0b11);
        const std::uint8_t length_timer = value & 0b111111;

        apu.set_channel_2_duty_cycle(duty);
        apu.set_channel_2_length_timer(length_timer);
    }

    export void write_channel_2_envelope_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const envelope_config envelope
        {
            static_cast<std::uint8_t>((value >> 4) & 0b1111),
            static_cast<envelope_direction>((value >> 3) & 0b1),
            static_cast<std::uint8_t>(value & 0b111)
        };

        apu.set_channel_2_envelope(envelope);
    }

    export void write_channel_2_period_low_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const std::uint16_t prev_period = apu.get_channel_2_period();
        const std::uint16_t new_period = (prev_period & (0b111 << 8)) | value;

        apu.set_channel_2_period(new_period);
    }

    export void write_channel_2_period_high_and_control_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const bool should_trigger = utils::is_bit_set<7>(value);
        const bool enable_length_timer = utils::is_bit_set<6>(value);
        const std::uint8_t period_high = value & 0b111;

        const std::uint16_t prev_period = apu.get_channel_2_period();
        const std::uint16_t new_period = (prev_period & 0xFF) | (period_high << 8);

        apu.set_channel_2_period(new_period);
        apu.enable_channel_2_length_timer(enable_length_timer);

        if (should_trigger)
        {
            apu.trigger_channel_2();
        }
    }

    // Channel 3 - Write
    export void write_channel_3_dac_enable_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const bool enable_dac = utils::is_bit_set<7>(value);
        apu.enable_channel_3_dac(enable_dac);
    }

    export void write_channel_3_length_timer_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        apu.set_channel_3_length_timer(value);
    }

    export void write_channel_3_output_level_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const auto output_level = static_cast<volume_level>((value >> 5) & 0b11);
        apu.set_channel_3_output_level(output_level);
    }

    export void write_channel_3_period_low_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const std::uint16_t prev_period = apu.get_channel_3_period();
        const std::uint16_t new_period = (prev_period & (0b111 << 8)) | value;

        apu.set_channel_3_period(new_period);
    }

    export void write_channel_3_period_high_and_control_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const bool should_trigger = utils::is_bit_set<7>(value);
        const bool enable_length_timer = utils::is_bit_set<6>(value);
        const std::uint8_t period_high = value & 0b111;

        const std::uint16_t prev_period = apu.get_channel_3_period();
        const std::uint16_t new_period = (prev_period & 0xFF) | (period_high << 8);

        apu.set_channel_3_period(new_period);
        apu.enable_channel_3_length_timer(enable_length_timer);

        if (should_trigger)
        {
            apu.trigger_channel_3();
        }
    }

    // Channel 4 - Write
    export void write_channel_4_length_timer_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        apu.set_channel_4_length_timer(value & 0b111111);
    }

    export void write_channel_4_envelope_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const envelope_config envelope
        {
            static_cast<std::uint8_t>((value >> 4) & 0b1111),
            static_cast<envelope_direction>((value >> 3) & 0b1),
            static_cast<std::uint8_t>(value & 0b111)
        };

        apu.set_channel_4_envelope(envelope);
    }

    export void write_channel_4_frequency_and_randomness_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const randomness_config random
        {
            static_cast<std::uint8_t>((value >> 4) & 0b1111),
            static_cast<lfsr_width>((value >> 3) & 0b1),
            static_cast<std::uint8_t>(value & 0b111)
        };

        apu.set_channel_4_randomness(random);
    }

    export void write_channel_4_control_address(audio_processing_unit& apu, const memory::memory_data_t value)
    {
        const bool should_trigger = utils::is_bit_set<7>(value);
        const bool enable_timer = utils::is_bit_set<6>(value);

        apu.enable_channel_4_length_timer(enable_timer);

        if (should_trigger)
        {
            apu.trigger_channel_4();
        }
    }

}
