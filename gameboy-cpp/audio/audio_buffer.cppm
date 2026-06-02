
export module audio:buffer;

import std;
import utilities;
import :common;

namespace audio
{
    export template<typename T>
    requires std::is_arithmetic_v<T>
    class audio_buffer
    {
    public:
        explicit audio_buffer(const std::size_t capacity)
            : buffer(capacity)
        {}

        template<typename Rep, typename Period>
        audio_buffer(const std::chrono::duration<Rep, Period>& duration, const std::uint32_t sample_rate, const std::uint8_t channels)
            : audio_buffer(sample_rate * channels * std::chrono::duration_cast<std::chrono::duration<float>>(duration).count())
        {}

        [[nodiscard]] size_t read_available() const { return buffer.read_available(); }
        [[nodiscard]] size_t write_available() const { return buffer.write_available(); }

        size_t write(std::span<const T> samples) { return buffer.push(samples); }
        size_t read(std::span<T> samples) { return buffer.pop(samples); }

    private:
        utils::ring_buffer<T> buffer;
    };

}
