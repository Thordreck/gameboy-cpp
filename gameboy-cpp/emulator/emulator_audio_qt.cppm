module;
#include <QAudioSink>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QMediaDevices>

export module emulator.audio:qt;

import std;
import audio;
import utilities;
import emulator.core;

namespace emulator
{
    export class audio_device
    {
    public:
        [[nodiscard]] std::uint32_t sample_rate() const { return format.sampleRate(); }
        [[nodiscard]] std::uint8_t channel_count() const { return format.channelCount(); }
        [[nodiscard]] bool is_open() const { return sink != nullptr; }

        void open()
        {
            using namespace std::chrono_literals;

            update_audio_config();

            sink = std::make_unique<QAudioSink>(format);
            buffer = std::make_unique<audio::audio_buffer<float>>(500ms, sample_rate(), channel_count());
            sink->start([this] (const QSpan<float> output) { audio_callback(*this->buffer, output); } );

            (utils::assert)(sink->error() == QtAudio::Error::NoError, "Could not open output audio device");

            if (is_muted)
            {
                sink->setVolume(0.0f);
            }
        }

        void close()
        {
            sink.reset();
            buffer.reset();
        }

        void resume()
        {
            if (sink != nullptr && sink->state() == QtAudio::SuspendedState)
            {
                sink->resume();
            }
        }

        void suspend()
        {
            if (sink != nullptr)
            {
                sink->suspend();
            }
        }

        void write(const std::span<const float> samples)
        {
            const std::size_t written = buffer != nullptr ? buffer->write(samples) : 0;
        }

        [[nodiscard]] volume_t volume() const
        {
            const qreal linear_volume = sink == nullptr
                ? 1.0f
                : is_muted ? muted_volume : sink->volume();

            const volume_t logarithmic_volume = QtAudio::convertVolume(
                static_cast<float>(linear_volume),
                QtAudio::LinearVolumeScale,
                QtAudio::LogarithmicVolumeScale);

            return logarithmic_volume;
        }

        void set_volume(const volume_t volume)
        {
            if (sink == nullptr)
            {
                return;
            }

            const qreal linear_volume = QtAudio::convertVolume(
                volume,
                QtAudio::LogarithmicVolumeScale,
                QtAudio::LinearVolumeScale);

            if (is_muted)
            {
                muted_volume = linear_volume;
            }
            else
            {
                sink->setVolume(linear_volume);
            }
        }

        [[nodiscard]] bool muted() const
        {
            return is_muted;
        }

        void set_muted(const bool updated_muted)
        {
            if (sink == nullptr || is_muted == updated_muted)
            {
                return;
            }

            is_muted = updated_muted;

            if (updated_muted)
            {
                muted_volume = sink->volume();
                sink->setVolume(0.0);
            }
            else
            {
                sink->setVolume(muted_volume);
            }
        }

    private:
        void update_audio_config()
        {
            const QAudioDevice default_device = QMediaDevices::defaultAudioOutput();
            format.setSampleRate(default_device.preferredFormat().sampleRate()) ;
            format.setChannelCount(2);
            format.setSampleFormat(QAudioFormat::Float);

            (utils::assert)(default_device.isFormatSupported(format), "Audio format not supported");
        }

        static void audio_callback(audio::audio_buffer<float>& buffer, QSpan<float> output)
        {
            const std::size_t samples_read = buffer.read(output);
            const std::size_t remaining_start = std::min(static_cast<std::size_t>(output.size()), samples_read);
            const auto remaining_slice = output.subspan(remaining_start);

            std::memset(remaining_slice.data(), 0, remaining_slice.size());
        };

        QAudioFormat format;
        std::unique_ptr<QAudioSink> sink { nullptr };
        std::unique_ptr<audio::audio_buffer<float>> buffer { nullptr };

        bool is_muted { false };
        qreal muted_volume {};
    };

}