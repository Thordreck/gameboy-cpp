module;
#include "doctest.h"

export module not_chciken;

import std;
import sdl;
import dsp;
import plot;

namespace not_chciken
{
	template<typename TExpected, typename TError>
	TExpected required(std::expected<TExpected, TError>&& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
		return std::forward<TExpected>(result.value());
	}

	[[nodiscard]] std::expected<std::filesystem::path, std::string> export_plot(
		plot::figure& figure,
		const std::string_view filename)
	{
		const std::filesystem::path output_filepath
			= std::filesystem::temp_directory_path()
			/ std::filesystem::path(filename).filename().replace_extension("svg");

		const auto result = plot::save_as_svg(figure, output_filepath);
		return result.transform([&output_filepath] { return output_filepath; });
	}

    export void run_square_preset_test(
    	const std::filesystem::path& expected_audio,
    	const std::uint8_t preset_idx)
    {
        // TODO: testing plot library
        using namespace plot;
        using namespace sdl;

		const auto audio_data = required(
			load_wav<float>(expected_audio)
			.and_then([] (const auto& wav_audio) { return convert_audio(wav_audio, wav_audio.specs().channels, 8000); })
			);

		const auto spec = audio_data.specs();

		std::println("Sample rate: {}", spec.sample_rate);
		figure fig {};

		// Time
		for (size_t channel = 0; channel < spec.channels; channel++)
		{
			const auto channel_samples = audio_data.channel(channel);

			plot_2d plot = fig.plot(0, channel);
			//plot.x().linear(0, duration);
			//plot.y().linear(-1, 1);

			line_2d waveform = plot.line();

			for (size_t i = 0; i < channel_samples.size(); ++i)
			{
				const double t = i / static_cast<double>(spec.sample_rate);
				waveform.add(t, channel_samples[i]);
			}
		}

		// FFT
		for (size_t channel = 0; channel < spec.channels; channel++)
		{
			const auto channel_samples = audio_data.channel(channel);
			const auto channel_fft = dsp::fft(channel_samples.data_handle(), channel_samples.size(), channel_samples.stride(0));

			plot_2d plot = fig.plot(1, channel);
			line_2d waveform = plot.line();

			for (size_t i = 0; i < channel_fft.size(); ++i)
			{
				const double t = i / static_cast<double>(spec.sample_rate);
				waveform.add(t, channel_fft[i].re);
			}
		}

		const auto image_path = required(export_plot(fig, "plot_test"));
		FAIL(std::format("Generated result image at {}", image_path.string()));
    }

}