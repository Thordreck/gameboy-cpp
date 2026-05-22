
export module emulator.audio:sdl;

import sdl;
import std;
import audio;
import utilities;

namespace emulator
{
    [[nodiscard]] sdl::audio_spec default_device_spec()
    {
        return utils::value_or_panic(sdl::get_audio_spec_from_id(sdl::default_output_device_id));
    }

    export class audio_device
    {
    public:
        [[nodiscard]] std::uint32_t sample_rate() const { return default_device_spec().sample_rate; }
        [[nodiscard]] std::uint8_t channel_count() const { return 2; }

        void open()
        {
            stream = utils::value_or_panic(sdl::open_output_audio_stream<float>(
                sdl::default_output_device_id,
                sample_rate(),
                channel_count()));
            utils::panic_on_error(stream->resume());
        }

        void close() { stream.reset(); }
        void write(const std::span<const float> samples) { utils::panic_on_error(stream->write(samples)); }

    private:
        std::optional<sdl::output_audio_stream<float>> stream { std::nullopt };
    };

}