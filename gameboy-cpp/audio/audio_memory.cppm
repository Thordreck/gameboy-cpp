
export module audio:memory;
import memory;

namespace audio
{
    export constexpr memory::memory_address_t channel_1_sweep_address = 0xFF10;
    export constexpr memory::memory_address_t channel_1_length_and_cycle_address = 0xFF11;
    export constexpr memory::memory_address_t channel_1_volume_and_envelope_address = 0xFF12;
    export constexpr memory::memory_address_t channel_1_period_low_address = 0xFF13;
    export constexpr memory::memory_address_t channel_1_period_high_and_control_address = 0xFF14;

    export constexpr memory::memory_address_t channel_2_length_and_cycle_address = 0xFF16;
    export constexpr memory::memory_address_t channel_2_volume_and_envelope_address = 0xFF17;
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

    export constexpr memory::memory_address_t wave_pattern_ram_start_address = 0xFF30;
    export constexpr memory::memory_address_t wave_pattern_ram_end_address = 0xFF3F;

}