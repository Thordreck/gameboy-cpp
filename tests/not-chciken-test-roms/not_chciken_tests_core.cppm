module;
#include "doctest.h"

export module not_chciken;

import std;
import mbc;
import dsp;
import sdl;
import svg;
import plot;
import joypad;
import engine;
import graphics;
import cartridge;
import utilities;
import tests.core;

namespace not_chciken
{
    template <typename Samples>
        requires std::ranges::contiguous_range<Samples> && sdl::AudioSample<std::ranges::range_value_t<Samples>>
    [[nodiscard]] std::expected<std::filesystem::path, std::string> export_audio(
        const Samples& samples,
        const std::uint8_t num_channels,
        const std::uint32_t sample_rate)
    {
        using namespace sdl;
        using namespace tests;

        const std::filesystem::path output_filepath = get_temp_file(".wav");

        return save_wav(samples, num_channels, sample_rate, output_filepath)
            .transform([&output_filepath] { return output_filepath; });
    }

    [[nodiscard]] std::expected<std::filesystem::path, std::string> export_plot(plot::figure& figure)
    {
        using namespace plot;
        using namespace svg;
        using namespace tests;

        std::stringstream svg_export_stream{};
        required(save_as_svg(figure, svg_export_stream));

        const std::filesystem::path output_filepath = get_temp_file(".png");

        return document::load(svg_export_stream.view().data(), svg_export_stream.str().size())
               .and_then([](auto doc) { return doc.to_bitmap(); })
               .and_then([&output_filepath](auto bitmap) { return bitmap.save_as_png(output_filepath); })
               .transform([&output_filepath] { return output_filepath; });
    }

    void populate_time_plot(
        plot::plot_2d& plot,
        plot::Indexable<double> auto const& samples,
        const std::uint32_t sample_rate)
    {
        using namespace plot;

        const auto audio_length = samples.size() / static_cast<double>(sample_rate);
        constexpr size_t num_x_ticks{10};
        const auto tick_period = audio_length / num_x_ticks;

        const auto t
            = std::views::iota(0uz, samples.size())
            | std::views::transform([sample_rate](const auto i) { return i / static_cast<double>(sample_rate); });

        const auto x_ticks
            = std::views::iota(0uz, num_x_ticks)
            | std::views::transform([tick_period](const auto i)
            {
                const double tick_value{(i + 1) * tick_period};
                return tick{tick_value, std::format("{:.1f}", tick_value)};
            });

        plot.line()
            .add(t, samples);

        plot.y()
            .minors(tick{-1}, tick{0}, tick{1});

        plot.x()
            .label("time (seconds)")
            .minor(tick{0})
            .minors(x_ticks);
    }

    void populate_spectrum_plot(
        plot::plot_2d& plot,
        plot::Indexable<float> auto const& spectrum,
        const float nfft,
        const std::uint32_t sample_rate)
    {
        using namespace plot;
        const auto max_fft_value = std::ranges::max(spectrum);

        const auto freqs
            = std::views::iota(0uz, static_cast<size_t>(spectrum.size()))
            | std::views::transform([sample_rate, nfft](const auto i)
            {
                return i * sample_rate / static_cast<double>(nfft);
            });

        constexpr int x_ticks_chunk_size{10};

        const auto x_ticks
            = std::views::enumerate(spectrum)
            | std::views::chunk(spectrum.size() / x_ticks_chunk_size)
            | std::views::transform([](const auto& window)
            {
                const auto compare_fft_y = [](const auto& lhs, const auto& rhs)
                {
                    return std::get<1>(lhs) < std::get<1>(rhs);
                };
                return std::ranges::max_element(window, compare_fft_y);
            })
            | std::views::filter([max_fft_value](const auto max_it)
            {
                const auto threshold = max_fft_value * 0.1f;
                return std::get<1>(*max_it) > threshold;
            })
            | std::views::transform([](const auto max_it) { return std::get<0>(*max_it); })
            | std::views::transform([sample_rate, nfft](const auto i)
            {
                const auto hz = i * sample_rate / static_cast<double>(nfft);
                return tick{hz, std::format("{}", std::round(hz))};
            });

        plot.line()
            .add(freqs, spectrum);

        // Note: it's not possible to iterate a const std::views::filter, since iterating it affects its internal state.
        // We can either remove the const from the x_ticks declaration or create a copy using the auto() operator.
        plot.x()
            .label("frequency (hz)")
            .minors(auto(x_ticks));

        plot.y()
            .minors(tick{0}, tick{max_fft_value, std::format("{:.2f}", max_fft_value)});
    }

    void populate_correlation_plot(
        plot::plot_2d& plot,
        plot::Indexable<double> auto const& correlation,
        const size_t original_length,
        const std::uint32_t sampling_frequency)
    {
        using namespace plot;

        const auto compute_lag = [original_length, sampling_frequency] (const size_t i)
        {
            return (static_cast<double>(i) - original_length - 1) / static_cast<double>(sampling_frequency);
        };

        const auto max_correlation_it = std::ranges::max_element(correlation);
        const auto max_correlation_index = std::ranges::distance(correlation.begin(), max_correlation_it);
        const auto x = std::views::iota(0uz, correlation.size()) | std::views::transform(compute_lag);
        const auto max_correlation_lag = compute_lag(max_correlation_index);

        plot.line()
            .add(x, correlation);

        plot.x()
            .minor(tick { max_correlation_lag, std::format("max = {:.2f} at lag = {:.1f} second)", *max_correlation_it, max_correlation_lag) });

        plot.y()
            .minors(tick { -1 }, tick { 0 }, tick { 1 } );
    }

    [[nodiscard]] sdl::audio_data<float> get_expected_audio(const std::filesystem::path& expected_audio_file)
    {
        using namespace sdl;
        using namespace tests;

        auto audio_data = required(load_wav<float>(expected_audio_file));

        REQUIRE_MESSAGE(audio_data.specs().channels <= 2, "Reference audio file must be mono or stereo");
        REQUIRE_MESSAGE(audio_data.specs().channels > 0, "Reference audio file must have at least one channel");

        return std::move(audio_data);
    }

    template <joypad::joypad_input Input>
    void activate_input(std::span<bool, joypad::num_joypad_inputs> inputs)
    {
        inputs[std::to_underlying(Input)] = true;
    }

    template <joypad::joypad_input Input>
    void trigger_input(engine::base_engine_ptr& engine)
    {
        using namespace joypad;
        using namespace engine;
        using namespace std::literals::chrono_literals;

        constexpr auto num_ticks_to_process_input = 16ms / tick_duration;

        std::array<bool, num_joypad_inputs> inputs{};

        activate_input<Input>(inputs);
        engine->update_joypad_state(inputs);
        engine->tick(num_ticks_to_process_input);

        inputs.fill(false);
        engine->update_joypad_state(inputs);
        engine->tick(num_ticks_to_process_input);
    }

    void trigger_preset(engine::base_engine_ptr& engine, const std::uint8_t preset_idx)
    {
        using namespace joypad;
        using enum joypad_input;

        // Position cursor over Play button
        constexpr auto num_down_presses = 12u;

        for (auto i = 0; i < num_down_presses; ++i)
        {
            trigger_input<down>(engine);
        }

        // Select preset
        for (auto i = 0; i < preset_idx; ++i)
        {
            trigger_input<select>(engine);
        }

        // Trigger audio
        trigger_input<a>(engine);
    }

    [[nodiscard]] std::vector<float> get_generated_audio(
        const std::filesystem::path& test_rom,
        const std::uint8_t preset_idx,
        const std::uint8_t num_channels,
        const std::uint32_t sample_rate,
        const std::chrono::duration<float>& expected_duration)
    {
        using namespace tests;
        using namespace engine;
        using namespace std::literals::chrono_literals;

        dummy_lcd lcd{};
        dummy_serial serial{};
        memory_audio audio{num_channels, sample_rate, false};

        auto cartridge = required(cartridge::load_rom_file(test_rom));
        auto engine = required(create_engine(cartridge, lcd, audio, serial));

        trigger_preset(engine, preset_idx);
        audio.open();

        const auto num_ticks = static_cast<std::uint32_t>((expected_duration / frame_duration) * num_ticks_per_frame);
        engine->tick(num_ticks);

        return audio.data();
    }

    template <sdl::AudioSample SampleType>
    [[nodiscard]] sdl::channel_span<const SampleType> get_audio_channel_view(
        const std::span<const SampleType> interleaved_samples,
        const std::size_t target_channel,
        const std::size_t total_channels)
    {
        return {interleaved_samples.data(), interleaved_samples.size(), total_channels, target_channel};
    }

    template <typename Container>
        requires std::ranges::sized_range<Container> && sdl::AudioSample<std::ranges::range_value_t<Container>>
    [[nodiscard]] auto get_audio_channel_view(
        Container&& interleaved_samples,
        const std::size_t target_channel,
        const std::size_t total_channels)
    {
        using sample_t = std::ranges::range_value_t<Container>;
        return get_audio_channel_view(std::span<const sample_t>(interleaved_samples), target_channel, total_channels);
    }

    export void run_square_preset_test(
        const std::filesystem::path& test_rom,
        const std::filesystem::path& expected_audio_file,
        const std::uint8_t preset_idx)
    {
        const auto expected_audio = get_expected_audio(expected_audio_file);
        const std::uint8_t channels = expected_audio.specs().channels;
        const std::uint32_t sample_rate = expected_audio.specs().sample_rate;
        const std::chrono::duration<float> expected_duration{
            expected_audio.samples().size() / channels / static_cast<float>(sample_rate)
        };
        const auto generated_audio = get_generated_audio(
            test_rom,
            preset_idx,
            channels,
            sample_rate,
            expected_duration
        );

        const auto correlation_scores
            = std::views::iota(0u, channels)
            | std::views::transform([&expected_audio, &generated_audio, channels](const auto channel)
            {
                const auto expected = expected_audio.channel(channel).deinterleaved();
                const auto generated = get_audio_channel_view(generated_audio, channel, channels).deinterleaved();

                const auto correlation = dsp::normalized_cross_correlation(expected, generated);

                return std::ranges::max(correlation);
            });

        const auto correlation_score = std::ranges::fold_left(correlation_scores, 0.0f, std::plus<>{}) / channels;

        if (correlation_score <= 0.98)
        {
            FAIL_CHECK(std::format("Generated audio differs too much from expected (correlation = {:2f})", correlation_score));

            plot::figure fig{};

            for (size_t channel = 0; channel < channels; ++channel)
            {
                const auto channel_name = channel == 0 ? "left" : "right";
                const auto expected_samples = expected_audio.channel(channel).deinterleaved();
                const auto generated_samples = get_audio_channel_view(generated_audio, channel, channels).deinterleaved();

                populate_time_plot(
                    fig.plot(0, channel).title(std::format("Expected {} channel audio", channel_name)),
                    expected_samples,
                    sample_rate);

                populate_time_plot(
                    fig.plot(1, channel).title(std::format("Generated {} channel audio", channel_name)),
                    generated_samples,
                    sample_rate);

                const auto expected_spectrum = dsp::spectrum(expected_samples);
                const auto generated_spectrum = dsp::spectrum(generated_samples);

                populate_spectrum_plot(
                    fig.plot(2, channel).title(std::format("Expected {} channel spectrum", channel_name)),
                    expected_spectrum,
                    dsp::nfft(expected_samples),
                    sample_rate);

                populate_spectrum_plot(
                    fig.plot(3, channel).title(std::format("Generated {} channel spectrum", channel_name)),
                    generated_spectrum,
                    dsp::nfft(generated_samples),
                    sample_rate);

                const auto correlation = dsp::normalized_cross_correlation(expected_samples, generated_samples);

                populate_correlation_plot(
                    fig.plot(4, channel).title(std::format("Normalized {} channel cross-correlation", channel_name)),
                    correlation,
                    expected_audio.specs().sample_rate,
                    expected_samples.size());
            }

            using namespace tests;

            const auto image_path = required(export_plot(fig));
            FAIL_CHECK(std::format("Generated result image at {}", image_path.string()));

            const auto audio_path = required(export_audio(generated_audio, channels, sample_rate));
            FAIL_CHECK(std::format("Generated result audio at {}", audio_path.string()));
        }
    }
}
