
export module audio:common;

import std;
import utilities;

namespace audio
{
    export using digital_sample = utils::constrained<std::uint8_t, 0x0, 0xF>;
    export using analog_sample = utils::constrained<float, -1, 1>;

    export template <typename T>
    struct stereo_sample
    {
        T left;
        T right;
    };

    export template <typename T>
    concept AudioSample = requires(T& sample)
    {
        typename T::underlying_type;
        std::is_arithmetic_v<typename T::underlying_type>;

        { T::min } -> std::convertible_to<typename T::underlying_type>;
        { T::max } -> std::convertible_to<typename T::underlying_type>;
        { static_cast<T::underlying_type>(sample) } -> std::convertible_to<typename T::underlying_type>;
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