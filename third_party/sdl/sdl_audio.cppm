module;
#include<SDL3/SDL_audio.h>

export module sdl:audio;

import std;
import :common;
import :internal;

namespace sdl
{
    export using audio_device_id = SDL_AudioDeviceID;
    export using audio_device_list_t = std::vector<audio_device_id>;

    export constexpr audio_device_id default_output_device_id { SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK };

    export enum class audio_format : std::uint32_t
    {
        unknown = SDL_AUDIO_UNKNOWN,
        u8 = SDL_AUDIO_U8,
        s8 = SDL_AUDIO_S8,
        s16_le = SDL_AUDIO_S16LE,
        s16_be = SDL_AUDIO_S16BE,
        s32_le = SDL_AUDIO_S32LE,
        s32_be = SDL_AUDIO_S32BE,
        f32_le = SDL_AUDIO_F32LE,
        f32_be = SDL_AUDIO_F32BE,

        #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        s16 = SDL_AUDIO_S16,
        s32 = SDL_AUDIO_S32,
        f32 = SDL_AUDIO_F32
        #else
        s16 = SDL_AUDIO_S16,
        s32 = SDL_AUDIO_S32,
        f32 = SDL_AUDIO_F32
        #endif
    };

    export struct audio_spec
    {
        audio_format format {};
        std::uint8_t channels {};
        std::uint32_t sample_rate {};
    };

    export result<audio_device_list_t> available_output_audio_devices()
    {
        int count { 0 };
        SDL_AudioDeviceID* devices = SDL_GetAudioPlaybackDevices(&count);

        if (devices == nullptr)
        {
            return std::unexpected(SDL_GetError());
        }

        audio_device_list_t results { devices, devices + count };
        SDL_free(devices);

        return results;
    }

    export template<internal::WrapperFor<SDL_AudioStream> Stream>
    [[nodiscard]] result<audio_device_id> get_audio_device_id(Stream& stream)
    {
        const audio_device_id id = SDL_GetAudioStreamDevice(internal::native::get_handle(stream));

        if (id == 0)
        {
            return std::unexpected { "Unbound or invalid stream" };
        }

        return id;
    }

    export [[nodiscard]] result<audio_spec> get_audio_spec_from_id(const audio_device_id id)
    {
        SDL_AudioSpec spec {};

        if (const bool success = SDL_GetAudioDeviceFormat(id, &spec, nullptr); !success)
        {
            return std::unexpected { SDL_GetError() };
        }

        return audio_spec
        {
            static_cast<audio_format>(spec.format),
            static_cast<std::uint8_t>(spec.channels),
            static_cast<std::uint32_t>(spec.freq)
        };
    }

    export template<internal::WrapperFor<SDL_AudioStream> Stream>
    [[nodiscard]] result<audio_spec> get_audio_spec(Stream& stream)
    {
        return get_audio_device_id(stream)
            .and_then([] (const audio_device_id id) { return get_audio_spec_from_id(id); });
    }

    export template<internal::WrapperFor<SDL_AudioStream> Stream>
    [[nodiscard]] result<void> resume_audio_stream(Stream& stream)
    {
        if (const bool success = SDL_ResumeAudioStreamDevice(internal::native::get_handle(stream)); !success)
        {
            return std::unexpected {  SDL_GetError() };
        }

        return {};
    }

    export template<internal::WrapperFor<SDL_AudioStream> Stream>
    [[nodiscard]] result<void> pause_audio_stream(Stream& stream)
    {
        if (const bool success = SDL_PauseAudioStreamDevice(internal::native::get_handle(stream)); !success)
        {
            return std::unexpected {  SDL_GetError() };
        }

        return {};
    }

    export template<typename T>
    concept AudioSample
        = std::is_same_v<T, std::uint8_t>
        || std::is_same_v<T, std::int8_t>
        || std::is_same_v<T, std::int16_t>
        || std::is_same_v<T, std::int32_t>
        || std::is_same_v<T, float>;

    export template<internal::WrapperFor<SDL_AudioStream> Stream, AudioSample Sample>
    [[nodiscard]] result<void> write_to_output_stream(Stream& stream, std::span<const Sample> samples)
    {
        const bool success = SDL_PutAudioStreamData(
            internal::native::get_handle(stream),
            samples.data(),
            samples.size() * sizeof(Sample));

        if (!success)
        {
            return std::unexpected { SDL_GetError() };
        }

        return {};
    }

    export template<AudioSample Sample>
    class output_audio_stream
    {
    public:
        [[nodiscard]] result<void> write(std::span<const Sample> samples) { return write_to_output_stream(*this, samples); }
        [[nodiscard]] result<void> resume() { return resume_audio_stream(*this); }
        [[nodiscard]] result<void> pause() { return pause_audio_stream(*this); }
        [[nodiscard]] result<audio_device_id> device_id() const { return get_audio_device_id(*this); }
        [[nodiscard]] result<audio_spec> spec() const { return get_audio_spec(*this); }

    private:
        explicit output_audio_stream(SDL_AudioStream* stream)
            : imp { stream, SDL_DestroyAudioStream }
        {}

        [[nodiscard]] auto native_handle() const { return imp.get(); }

        std::unique_ptr<SDL_AudioStream, decltype(&SDL_DestroyAudioStream)> imp;

        friend internal::native;
        friend internal::wrapper;
    };

    export template<AudioSample Sample>
    [[nodiscard]] result<output_audio_stream<Sample>> open_output_audio_stream(
        const audio_device_id id,
        const audio_spec& spec)
    {
        const SDL_AudioSpec sdl_spec
        {
            static_cast<SDL_AudioFormat>(std::to_underlying(spec.format)),
            spec.channels,
            static_cast<int>(spec.sample_rate)
        };

        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(id, &sdl_spec, nullptr, nullptr);

        if (stream == nullptr)
        {
            return std::unexpected { SDL_GetError() };
        }

        return internal::wrapper::create<output_audio_stream<Sample>>(stream);
    }

    export template<AudioSample Sample>
    [[nodiscard]] result<output_audio_stream<Sample>> open_output_audio_stream(
        const audio_device_id id,
        const std::uint32_t sample_rate,
        const std::uint8_t channels)
    {
        return get_audio_spec_from_id(id)
            .transform([&] (auto spec)
            {
                spec.channels = channels;
                spec.sample_rate = sample_rate;

                return spec;
            })
            .and_then([id] (const auto spec) { return open_output_audio_stream<Sample>(id, spec); });
    }

}