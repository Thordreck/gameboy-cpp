
export module audio:common;

import std;
import utilities;

namespace audio
{
    export using digital_sample = utils::constrained<std::uint8_t, 0x0, 0xF>;
    export using analog_sample = utils::constrained<float, -1, 1>;

    export template <typename T>
    struct stereo
    {
        T left;
        T right;
    };

    export using stereo_panning = stereo<bool>;
    export using stereo_volume = stereo<std::uint8_t>;

    export enum class sweep_direction : std::uint8_t
    {
        addition = 0,
        substraction = 1
    };

    export constexpr std::uint8_t num_apu_channels = 4;

    export template <typename T>
    using channel_array_t = std::array<T, num_apu_channels>;

    export struct sweep_config
    {
        std::uint8_t pace {};
        sweep_direction direction {};
        std::uint8_t step {};
    };

    export enum class duty_cycle : std::uint8_t
    {
        _12_5 = 0b00,
        _25 = 0b01,
        _50 = 0b10,
        _75 = 0b11,
    };

    export enum class envelope_direction : std::uint8_t
    {
        decrease = 0,
        increase = 1,
    };

    export struct envelope_config
    {
        std::uint8_t volume {};
        envelope_direction direction {};
        std::uint8_t pace {};
    };

    export enum class volume_level : std::uint8_t
    {
        mute = 0b00,
        full = 0b01,
        half = 0b10,
        quarter = 0b11,
    };

    export enum class lfsr_width : std::uint8_t
    {
        _15_bit = 0,
        _7_bit = 1,
    };

    export struct randomness_config
    {
        std::uint8_t clock_shift {};
        lfsr_width lfsr_width {};
        std::uint8_t clock_divider {};
    };

    export constexpr std::uint8_t wave_ram_size = 0xF;
    export using wave_ram_view_t = std::span<std::uint8_t, wave_ram_size>;
    export using const_wave_ram_view_t = std::span<const std::uint8_t, wave_ram_size>;

    export template <typename T>
    concept AudioSample = requires(T& sample)
    {
        typename T::underlying_type;
        std::is_arithmetic_v<typename T::underlying_type>;

        { T::min } -> std::convertible_to<typename T::underlying_type>;
        { T::max } -> std::convertible_to<typename T::underlying_type>;
        { sample.data() } -> std::convertible_to<typename T::underlying_type>;
    };

    export template <typename T>
    concept AudioInterface = requires(const T& imp)
    {
        { imp.sample_rate() } -> std::convertible_to<std::uint32_t>;
        { imp.channel_count() } -> std::convertible_to<std::uint8_t>;
    };

    export template <typename Sink, typename Sample>
    concept AudioSink = AudioInterface<Sink> && std::is_arithmetic_v<Sample> && requires(Sink& sink, std::span<const Sample> samples)
    {
        { sink.write(samples) } -> std::same_as<void>;
    };

    export template <typename Source, typename Sample>
    concept AudioSource = AudioInterface<Source> && std::is_arithmetic_v<Sample> && requires(Source& source, std::span<Sample> samples)
    {
        { source.read(samples) } -> std::convertible_to<size_t>;
    };

    export template <typename T, typename Sample>
    concept AudioBuffer = AudioSink<T, Sample> && AudioSource<T, Sample>;

}
