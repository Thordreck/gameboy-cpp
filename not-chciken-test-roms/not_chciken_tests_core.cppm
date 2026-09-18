module;
#include "doctest.h"

export module not_chciken;

import std;
import dsp;
import sdl;
import svg;
import plot;

namespace not_chciken
{
	template<typename TExpected, typename TError>
	TExpected required(std::expected<TExpected, TError>&& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
		return std::forward<TExpected>(result.value());
	}

	template<typename TError>
	void required(std::expected<void, TError>&& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
	}

	[[nodiscard]] std::expected<std::filesystem::path, std::string> export_plot(
		plot::figure& figure,
		const std::string_view filename)
	{
		using namespace plot;
		using namespace svg;

		std::stringstream svg_export_stream {};
		required(save_as_svg(figure, svg_export_stream));

		const std::filesystem::path output_filepath
			= std::filesystem::temp_directory_path()
			/ std::filesystem::path(filename).filename().replace_extension("png");

		return document::load(svg_export_stream.view().data(), svg_export_stream.str().size())
			.and_then([] (auto doc) { return doc.to_bitmap(); })
			.and_then([&output_filepath] (auto bitmap) { return bitmap.save_as_png(output_filepath); })
			.transform([&output_filepath] { return output_filepath; });
	}

    export void run_square_preset_test(
    	const std::filesystem::path& expected_audio,
    	const std::uint8_t preset_idx)
    {
        // TODO: testing plot library
        using namespace plot;
        using namespace sdl;
		using namespace dsp;

		const auto audio_data = required(
			load_wav<float>(expected_audio)
			.and_then([] (const auto& wav_audio) { return convert_audio(wav_audio, wav_audio.specs().channels, 8000); })
			);

		const auto spec = audio_data.specs();
		REQUIRE_MESSAGE(spec.channels <= 2, "Reference audio file must be mono or stereo");

		figure fig {};

		// Time
		for (size_t channel = 0; channel < spec.channels; channel++)
		{
			plot_2d plot = fig.plot(0, channel);
			plot.title(std::format("{} channel: time", channel == 0 ? "Left" : "Right"));

			const auto channel_samples = audio_data.channel(channel);
			const auto t
				= std::views::iota(0uz, channel_samples.size())
				| std::views::transform([&spec] (const auto i) { return i / static_cast<double>(spec.sample_rate); });

			line_2d waveform = plot.line();
			waveform.add(t, channel_samples);

			plot.y()
				.minors( tick { -1}, tick { 0 }, tick { 1 } );

			/*
			const auto x_ticks
				= std::views::iota(0, channel_samples.size() / static_cast<double>(spec.sample_rate))
				| std::views::transform([] (const auto i) { return tick { static_cast<double>(i) }; });

			for (auto tick : x_ticks)
			{
				std::println("Tick {}", tick.value());
			}

			plot.x()
				.label("time (seconds)")
				.minors(x_ticks);
			*/
		}

		// FFT
		for (size_t channel = 0; channel < spec.channels; channel++)
		{
			const auto channel_samples = audio_data.channel(channel);
			const auto hamming_window = hamming(channel_samples.size());

			const auto windowed_samples = hamming_window * channel_samples;
			const auto full_channel_fft = fft(windowed_samples);
			const auto shifted_channel_fft = fft_shift(full_channel_fft);
			const auto channel_spectrum = abs(shifted_channel_fft.slice(0, full_channel_fft.size() / 2));

			plot_2d plot = fig.plot(1, channel);
			line_2d waveform = plot.line();

			for (size_t i = 0; i < channel_spectrum.size(); ++i)
			{
				//const double t = i / static_cast<double>(spec.sample_rate);
				waveform.add(i, channel_spectrum[i]);
			}
		}

		// Cross correlation
		for (size_t channel = 0; channel < spec.channels; channel++)
		{
			const auto channel_samples = audio_data.channel(channel);
			const auto channel_array = make_array(channel_samples.data_handle(), channel_samples.size(), channel_samples.stride(0));
			const auto channel_correlation = abs(auto_correlation(channel_array));

			plot_2d plot = fig.plot(2, channel);
			line_2d waveform = plot.line();

			for (size_t i = 0; i < channel_correlation.size(); ++i)
			{
				const double t = i / static_cast<double>(spec.sample_rate);
				waveform.add(i, channel_correlation[i]);
			}
		}

		const auto image_path = required(export_plot(fig, "plot_test"));
		FAIL(std::format("Generated result image at {}", image_path.string()));
    }

}