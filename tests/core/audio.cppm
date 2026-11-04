
export module tests.core:audio;
import std;

namespace tests
{
    export class dummy_audio
    {
    public:
        static [[nodiscard]] std::uint8_t channel_count() { return 2; }
        static [[nodiscard]] std::uint32_t sample_rate() { return 44100; }
        static void write(const std::span<const float>) {}
    };

    export class memory_audio
    {
    public:
        memory_audio(const std::uint8_t channels, const std::uint32_t rate, const bool open)
            : num_channels { channels }
            , sampling_rate { rate }
            , is_open { open }
        {}

        [[nodiscard]] std::uint8_t channel_count() const { return num_channels; }
        [[nodiscard]] std::uint32_t sample_rate() const { return sampling_rate; }
        [[nodiscard]] std::vector<float> data() const { return samples; }

        void open() { is_open = true; }
        void close() { is_open = false; }

        void write(const std::span<const float> input)
        {
            if (is_open)
            {
                samples.insert(samples.end(), input.begin(), input.end());
            }
        }

    private:
        std::uint8_t num_channels;
        std::uint32_t sampling_rate;
        bool is_open;

        std::vector<float> samples {};
    };

}