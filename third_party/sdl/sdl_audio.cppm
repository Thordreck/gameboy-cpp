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
        = std::is_same_v<std::remove_cvref_t<T>, std::uint8_t>
        || std::is_same_v<std::remove_cvref_t<T>, std::int8_t>
        || std::is_same_v<std::remove_cvref_t<T>, std::int16_t>
        || std::is_same_v<std::remove_cvref_t<T>, std::int32_t>
        || std::is_same_v<std::remove_cvref_t<T>, float>;

    export template<AudioSample T>
    consteval audio_format associated_audio_format()
    {
        using enum audio_format;

        if constexpr (std::is_same_v<T, std::uint8_t>)
            return u8;
        else if constexpr (std::is_same_v<T, std::int8_t>)
            return s8;
        else if constexpr (std::is_same_v<T, std::int16_t>)
            return s16;
        else if constexpr (std::is_same_v<T, std::int32_t>)
            return s32;
        else if constexpr (std::is_same_v<T, float>)
            return f32;

        std::unreachable();
    }

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

    export template<AudioSample SampleType>
    using channel_span = std::mdspan<SampleType, std::dextents<std::size_t, 1>, std::layout_stride>;

    export template<AudioSample SampleType>
    class audio_data
    {
    public:
        std::span<SampleType> samples() { return { data.get(), length }; }
        std::span<const SampleType> samples() const { return { data.get(), length }; }

        channel_span<SampleType> channel(const std::size_t n)
        {
            std::extents shape { length / spec.channels };
            std::array stride { spec.channels };

            return { data.get() + n, { shape, stride } };
        }

        channel_span<const SampleType> channel(const std::size_t n) const
        {
            std::extents shape { length / spec.channels };
            std::array stride { spec.channels };

            return { data.get() + n, { shape, stride } };
        }

        channel_span<SampleType> left() { return channel(0); }
        channel_span<const SampleType> left() const { return channel(0); }

        channel_span<SampleType> right() { return channel(1); }
        channel_span<const SampleType> right() const { return channel(1); }

        audio_spec specs() const { return spec; }

    private:
        audio_data(SampleType* data, const std::size_t length, const SDL_AudioSpec& spec)
            : data { data, SDL_free }
            , length { length }
            , spec { static_cast<audio_format>(spec.format), static_cast<std::uint8_t>(spec.channels), static_cast<std::uint32_t>(spec.freq) }
        {}

        std::unique_ptr<SampleType, decltype(&SDL_free)> data;
        std::size_t length;
        audio_spec spec;

        friend internal::wrapper;
    };

    export template<AudioSample SampleType>
    [[nodiscard]] result<audio_data<SampleType>> load_wav(const std::filesystem::path& path)
    {
        SDL_AudioSpec wav_spec {};
        std::uint8_t* wav_samples { nullptr };
        std::uint32_t wav_length {};

        if (const bool load = SDL_LoadWAV(path.string().c_str(), &wav_spec, &wav_samples, &wav_length); !load)
        {
            return std::unexpected { SDL_GetError() };
        }

        const defer cleanup { [wav_samples] { SDL_free(wav_samples); }};

        SDL_AudioSpec converted_spec
        {
            static_cast<SDL_AudioFormat>(associated_audio_format<SampleType>()),
            wav_spec.channels,
            wav_spec.freq
        };

        std::uint8_t* converted_samples { nullptr };
        std::uint32_t converted_length {};

        if (const bool convert = SDL_ConvertAudioSamples(
            &wav_spec,
            wav_samples,
            wav_length,
            &converted_spec,
            &converted_samples,
            reinterpret_cast<int*>(&converted_length)); !convert)
        {
            return std::unexpected { SDL_GetError() };
        }

        return internal::wrapper::create<audio_data<SampleType>>(
            reinterpret_cast<SampleType*>(converted_samples),
            converted_length / sizeof(SampleType),
            converted_spec);
    }

    export template<typename Samples>
    requires std::ranges::contiguous_range<Samples> && AudioSample<std::ranges::range_value_t<Samples>>
    [[nodiscard]] outcome save_wav(
        const Samples& samples,
        const std::uint8_t num_channels,
        const std::uint32_t sample_rate,
        const std::filesystem::path& file)
    {
        SDL_IOStream* output_stream = SDL_IOFromFile(file.string().c_str(), "wb");

        if (output_stream == nullptr)
        {
            return std::unexpected { SDL_GetError() };
        }

        const defer cleanup { [output_stream] { SDL_CloseIO(output_stream); }};

        using enum audio_format;
        using sample_t = std::ranges::range_value_t<Samples>;

        const audio_format format = associated_audio_format<sample_t>();
        const auto bits = SDL_AUDIO_BITSIZE(std::to_underlying(format));
        const auto bytes_per_sample = bits / 8;
        const auto byte_rate = sample_rate * num_channels * bytes_per_sample;
        const auto block_align = num_channels * bytes_per_sample;
        const auto sample_count = std::ranges::size(samples);
        const auto length = sample_count * sizeof(sample_t);
        const auto buffer = std::ranges::data(samples);
        const auto wav_format = (format == f32 || format == f32_be || format == f32_le) ? 3 : 1;
        const auto require = [](const bool condition) { return condition ? outcome{} : std::unexpected { SDL_GetError() }; };

        return require(SDL_WriteU32LE(output_stream, 0x46464952)) // RIFF
            .and_then([output_stream, &require, length] { return require(SDL_WriteU32LE(output_stream, 36 + length)); })
            .and_then([output_stream, &require] { return require(SDL_WriteU32LE(output_stream, 0x45564157)); }) // WAVE
            .and_then([output_stream, &require] { return require(SDL_WriteU32LE(output_stream, 0x20746D66)); }) // fmt
            .and_then([output_stream, &require] { return require(SDL_WriteU32LE(output_stream, 16)); })
            .and_then([output_stream, &require, wav_format] { return require(SDL_WriteU16LE(output_stream, wav_format)); })
            .and_then([output_stream, &require, num_channels] { return require(SDL_WriteU16LE(output_stream, num_channels)); })
            .and_then([output_stream, &require, sample_rate] { return require(SDL_WriteU32LE(output_stream, sample_rate)); })
            .and_then([output_stream, &require, byte_rate] { return require(SDL_WriteU32LE(output_stream, byte_rate)); })
            .and_then([output_stream, &require, block_align] { return require(SDL_WriteU16LE(output_stream, block_align)); })
            .and_then([output_stream, &require, bits] { return require(SDL_WriteU16LE(output_stream, bits)); })
            .and_then([output_stream, &require] { return require(SDL_WriteU32LE(output_stream, 0x61746164)); }) // data
            .and_then([output_stream, &require, length] { return require(SDL_WriteU32LE(output_stream, length)); })
            .and_then([output_stream, &require, buffer, length] { return require(SDL_WriteIO(output_stream, buffer, length) == length); });
    }

    export template<AudioSample SampleType>
    [[nodiscard]] outcome save_wav(const audio_data<SampleType>& samples, const std::filesystem::path& file)
    {
        return save_wav(samples.data(), samples.specs().channels, samples.specs().sample_rate, file);
    }

    export template<AudioSample From, AudioSample To>
    [[nodiscard]] result<audio_data<To>> convert_audio(
        const audio_data<From>& source,
        const std::uint8_t channels,
        const std::uint32_t sample_rate)
    {
        const SDL_AudioSpec src_spec
        {
            static_cast<SDL_AudioFormat>(associated_audio_format<From>()),
            source.specs().channels,
            static_cast<int>(source.specs().sample_rate)
        };

        const auto src_samples = source.samples();
        const size_t src_length { src_samples.size_bytes() };

        const SDL_AudioSpec converted_spec
        {
            static_cast<SDL_AudioFormat>(associated_audio_format<To>()),
            channels,
            static_cast<int>(sample_rate)
        };

        std::uint8_t* converted_samples { nullptr };
        std::uint32_t converted_length {};

        if (const bool convert = SDL_ConvertAudioSamples(
            &src_spec,
            reinterpret_cast<const std::uint8_t*>(src_samples.data()),
            static_cast<int>(src_length),
            &converted_spec,
            &converted_samples,
            reinterpret_cast<int*>(&converted_length)); !convert)
        {
            return std::unexpected { SDL_GetError() };
        }

        return internal::wrapper::create<audio_data<To>>(
            reinterpret_cast<To*>(converted_samples),
            converted_length / sizeof(To),
            converted_spec);
    }

    export template<AudioSample SampleType>
    [[nodiscard]] result<audio_data<SampleType>> convert_audio(
        const audio_data<SampleType>& source,
        const std::uint8_t channels,
        const std::uint32_t sample_rate)
    {
        return convert_audio<SampleType, SampleType>(source, channels, sample_rate);
    }

}