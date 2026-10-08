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

// TODO: remove once done debugging
import stb_image;

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

	void populate_time_domain_plot(
		plot::plot_2d&& plot,
		plot::Indexable<double> auto&& samples,
		const std::uint32_t sample_rate)
	{
        using namespace plot;

		const auto audio_length = samples.size() / static_cast<double>(sample_rate);
		constexpr int num_x_ticks { 10 };
		const auto tick_period = audio_length / num_x_ticks;

		const auto t
			= std::views::iota(0uz, samples.size())
			| std::views::transform([sample_rate] (const auto i) { return i / static_cast<double>(sample_rate); });

		const auto x_ticks
			= std::views::iota(0, num_x_ticks)
			| std::views::transform([tick_period] (const auto i)
			{
				const double tick_value { (i + 1) * tick_period };
				return tick { tick_value, std::format("{:.1f}", tick_value) };
			});

		plot.title("Time")
			.line()
			.add(t, samples);

		plot.y()
			.minors( tick { -1}, tick { 0 }, tick { 1 } );

		plot.x()
			.label("time (seconds)")
			.minor(tick{ 0 })
			.minors(x_ticks);
	}

	void populate_frequency_domain_plot(
		plot::plot_2d&& plot,
		plot::Indexable<double> auto&& samples,
		const std::uint32_t sample_rate)
	{
		using namespace dsp;
		using namespace plot;

		const auto hamming_window = hamming(samples.size());
		const auto windowed_samples = hamming_window * samples;
		const auto nfft = windowed_samples.size();
		const auto full_channel_fft = fft(windowed_samples) / windowed_samples.size();
		const auto channel_spectrum = abs(full_channel_fft.slice(0, full_channel_fft.size() / 2));
		const auto max_fft_value = std::ranges::max(channel_spectrum);

		const auto freqs
			= std::views::iota(0, channel_spectrum.size())
			| std::views::transform([sample_rate, nfft] (const auto i) { return i * sample_rate / static_cast<double>(nfft); });

		constexpr int x_ticks_chunk_size { 10 };

		const auto x_ticks
			= std::views::enumerate(channel_spectrum)
			| std::views::chunk(channel_spectrum.size() / x_ticks_chunk_size)
			| std::views::transform([] (const auto& window)
				{
					const auto compare_fft_y = [] (const auto& lhs, const auto& rhs) { return std::get<1>(lhs) < std::get<1>(rhs); };
					return std::ranges::max_element(window, compare_fft_y);
				})
			| std::views::filter([max_fft_value] (const auto max_it)
				{
					const auto threshold = max_fft_value * 0.1f;
					return std::get<1>(*max_it) > threshold;
				})
			| std::views::transform([] (const auto max_it) { return std::get<0>(*max_it); })
			| std::views::transform([sample_rate, nfft] (const auto i)
				{
					const auto hz = i * sample_rate / static_cast<double>(nfft);
					return tick { hz , std::format("{}", std::round(hz)) };
				});

		plot.title("Frequency")
			.line()
			.add(freqs, channel_spectrum);

		// Note: it's not possible to iterate a const std::views::filter, since iterating it affects its internal state.
		// We can either remove the const from the x_ticks declaration or create a copy using the auto() operator.
		plot.x()
			.label("frequency (hz)")
			.minors(auto(x_ticks));

		plot.y()
			.minors(tick { 0 }, tick { max_fft_value, std::format("{:.2f}", max_fft_value) } );
	}

	void populate_correlation_plot(
		plot::plot_2d&& plot,
		plot::Indexable<double> auto&& lhs,
		plot::Indexable<double> auto&& rhs)
	{
		using namespace plot;
		using namespace dsp;

		const auto lhs_array = make_array(lhs.data_handle(), lhs.size(), lhs.stride(0));
		const auto rhs_array = make_array(rhs.data_handle(), rhs.size(), rhs.stride(0));
		const auto channel_correlation = abs(cross_correlation(lhs_array, rhs_array));
		const auto max_correlation_it = std::ranges::max_element(channel_correlation);
		const auto max_correlation_index = std::ranges::distance(channel_correlation.begin(), max_correlation_it);
		const auto x = std::views::iota(0, channel_correlation.size());

		plot.title("Cross-correlation")
			.line()
			.add(x, channel_correlation);

		plot.x()
			.minor(tick { static_cast<double>(max_correlation_index), "max" });

		plot.y()
			.minors(tick { 0 }, tick { *max_correlation_it, std::format("{:.2f}", *max_correlation_it) } );
	}

	sdl::audio_data<float> get_expected_audio(const std::filesystem::path& expected_audio_file)
	{
		using namespace sdl;

		auto audio_data = required(
			load_wav<float>(expected_audio_file)
			.and_then([] (const auto& wav_audio)
				{
					return convert_audio(wav_audio, wav_audio.specs().channels, 8000);
				})
			);

		REQUIRE_MESSAGE(audio_data.specs().channels <= 2, "Reference audio file must be mono or stereo");
		REQUIRE_MESSAGE(audio_data.specs().channels > 0, "Reference audio file must have at least one channel");

		return std::move(audio_data);
	}

	struct dummy_serial
	{
		[[nodiscard]] static std::uint8_t transfer_bit(const std::uint8_t) { return 0x00; }
	};

	struct dummy_lcd
	{
		void write_frame(const graphics::lcd_memory_view_t new_frame)
		{
			std::ranges::copy(new_frame.begin(), new_frame.end(), frame.begin());

			const std::filesystem::path output_filepath
				= std::filesystem::temp_directory_path()
			/ std::format("square-test-frame-{}.png", num_frame++);

			constexpr stb::image_metadata output_metadata
			{
				graphics::lcd_width,
				graphics::lcd_height,
				graphics::num_color_channels
			};

			required(stb::write_png(output_filepath, frame.data(), output_metadata));
		}

	private:
		graphics::lcd_memory_t frame {};
		std::size_t num_frame {};
	};

	template<typename T>
	concept AudioSinkBuffer = requires(T buffer, std::span<const float> samples)
	{
		{ buffer.push(samples) } -> std::convertible_to<size_t>;
	};

	template<AudioSinkBuffer Buffer>
	class test_audio_sink
	{
	public:
		test_audio_sink(Buffer& buffer, const std::uint8_t num_channels, const std::uint32_t sample_rate)
			: buffer { buffer }
			, num_channels { num_channels }
			, audio_sample_rate { sample_rate }
		{}

		[[nodiscard]] std::uint8_t channel_count() const { return num_channels; }
		[[nodiscard]] std::uint32_t sample_rate() const { return audio_sample_rate; }

		void enable_capture() { capture_enabled = true; }
		void disable_capture() { capture_enabled = false; }

		void write(const std::span<const float> samples)
		{
			if (capture_enabled)
			{
				const auto num_samples_written = buffer.push(samples);
				REQUIRE_MESSAGE(num_samples_written == samples.size(), "Could not write all audio samples into buffer");
			}
		}

	private:
		Buffer& buffer;
		std::uint8_t num_channels;
		std::uint32_t audio_sample_rate;

		bool capture_enabled { false };
	};

	template<joypad::joypad_input Input>
	void activate_input(std::span<bool, joypad::num_joypad_inputs> inputs)
	{
		inputs[std::to_underlying(Input)] = true;
	}

	template<joypad::joypad_input Input>
	void trigger_input(engine::base_engine_ptr& engine)
	{
		using namespace joypad;
		using namespace engine;
		using namespace std::literals::chrono_literals;

		constexpr auto num_ticks_to_process_input = 16ms / tick_duration;

		std::array<bool, num_joypad_inputs> inputs {};

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

	std::vector<float> get_generated_audio(
		const std::filesystem::path& test_rom,
		const std::uint8_t preset_idx,
		const std::uint8_t num_channels,
		const std::uint32_t sample_rate,
		const std::chrono::seconds& expected_duration)
	{
		using namespace engine;
		using namespace std::literals::chrono_literals;

		dummy_lcd lcd {};
		dummy_serial serial {};

		const size_t buffer_capacity = std::llroundl(((expected_duration + 0.5s) / 1.0s) * sample_rate * num_channels);
		utils::ring_buffer<float> audio_buffer { buffer_capacity };

		test_audio_sink audio { audio_buffer, num_channels, sample_rate };

		auto cartridge = required(cartridge::load_rom_file(test_rom));
		auto engine = required(create_engine(cartridge, lcd, audio, serial));

		trigger_preset(engine, preset_idx);
		audio.enable_capture();

		const size_t num_ticks = (expected_duration / frame_duration) * num_ticks_per_frame;
		engine->tick(num_ticks);

		std::vector<float> generated_samples(audio_buffer.read_available());
		const size_t num_samples_read = audio_buffer.pop(generated_samples);
		REQUIRE_MESSAGE(num_samples_read == generated_samples.size(), "Could not read all audio samples from buffer");

		return generated_samples;
	}

	template<sdl::AudioSample SampleType>
	sdl::channel_span<const SampleType> get_audio_channel_view(
		const std::span<const SampleType> interleaved_samples,
		const std::size_t target_channel,
		const std::size_t total_channels)
	{
		std::extents shape { interleaved_samples.size() / total_channels };
		std::array stride { total_channels };

		return { interleaved_samples.data() + target_channel, { shape, stride } };
	}

	template<typename Container>
	requires std::ranges::sized_range<Container> && sdl::AudioSample<std::ranges::range_value_t<Container>>
	auto get_audio_channel_view(
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
		using namespace std::literals::chrono_literals;

		const auto expected_audio = get_expected_audio(expected_audio_file);
		const std::uint8_t channels = expected_audio.specs().channels;
		const std::uint32_t sample_rate = expected_audio.specs().sample_rate;
		const auto generated_audio = get_generated_audio(
			test_rom,
			preset_idx,
			channels,
			sample_rate,
			3s
			);

		// TODO: compute cross-correlation to check similarity score
		plot::figure fig {};

		for (size_t channel = 0; channel < channels; ++channel)
		{
			const auto expected_samples = expected_audio.channel(channel);
			const auto generated_samples = get_audio_channel_view(generated_audio, channel, channels);

			populate_time_domain_plot(fig.plot(0, channel), expected_samples, sample_rate);
			populate_time_domain_plot(fig.plot(1, channel), generated_samples, sample_rate);

			populate_frequency_domain_plot(fig.plot(2, channel), expected_samples, sample_rate);
			populate_frequency_domain_plot(fig.plot(3, channel), generated_samples, sample_rate);

			populate_correlation_plot(fig.plot(4, channel), expected_samples, generated_samples);
		}

		const auto image_path = required(export_plot(fig, "plot_test"));
		FAIL(std::format("Generated result image at {}", image_path.string()));
    }

}