
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

}