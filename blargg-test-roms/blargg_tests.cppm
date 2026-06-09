module;
#include "doctest.h"

export module blargg;

import std;
import mbc;
import memory;
import graphics;
import utilities;
import cartridge;
import stb_image;
import emulator.core;
import emulator.engine;
import emulator.gameboy;

namespace blargg
{
	template<typename TExpected, typename TError>
	TExpected require_success(std::expected<TExpected, TError>&& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
		return std::forward<TExpected>(result.value());
	}

	template<memory::ReadOnlyMemory Memory>
	std::string decode_result_from_memory(
		const memory::memory_address_t start,
		const memory::memory_address_t end,
		const Memory& memory)
	{
		std::string result {};

		for (auto address = start; address < end; address++)
		{
			const auto value = memory.read(address);
			if (value == '\0') { break; }

			result += value;
		}

		return result;
	}

	template<emulator::Engine Engine>
	[[nodiscard]] std::expected<std::filesystem::path, std::string> export_lcd(
		const Engine& engine,
		const std::string_view rom_file_path)
	{
		const std::filesystem::path output_filepath
			= std::filesystem::temp_directory_path()
			/ std::filesystem::path(rom_file_path).filename().replace_extension("png");

		constexpr stb::image_metadata output_metadata
		{
			graphics::lcd_width,
			graphics::lcd_height,
			graphics::num_color_channels
		};

		const auto result = stb::write_png(output_filepath, engine.lcd().data(), output_metadata);
		return result.transform([output_filepath] { return output_filepath; });
	}

	class test_serial
	{
	public:
		[[nodiscard]] std::string result() const
		{
			return { buffer.begin(), buffer.end() };
		}

		[[nodiscard]] std::uint8_t transfer_bit(const std::uint8_t bit)
		{
			current_byte = current_byte << 1  | (bit & 0b1);

			if (++bits_received >= 8)
			{
				buffer.push_back(current_byte);
				bits_received = 0;
			}

			return 0x00;
		}

	private:
		std::vector<std::uint8_t> buffer {};
		std::uint8_t current_byte {};
		std::uint8_t bits_received {};
	};

	class test_audio_sink
	{
	public:
		static [[nodiscard]] std::uint8_t channel_count() { return 2; }
		static [[nodiscard]] std::uint32_t sample_rate() { return 44100; }
		static void write(const std::span<const float>) {}
	};

	export void run_serial_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output,
		const size_t num_t_cycles)
	{
		test_serial serial {};
		test_audio_sink audio_sink {};

		auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
		auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));

		engine->tick(num_t_cycles);
		const std::string result = serial.result();
		const bool received_expected_result = result == expected_output;

		if (!received_expected_result)
		{
			const auto image_path = require_success(export_lcd(*engine, rom_file_path));
			FAIL(std::format("Incorrect result received: {}\nGenerated result image at {}", result, image_path.string()));
		}

		std::cout << result;
	}

	export void run_memory_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output,
		const size_t num_t_cycles)
	{
		test_serial serial {};
		test_audio_sink audio_sink {};

		auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
		auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));

		engine->tick(num_t_cycles);
		const auto memory = engine->memory();

		const std::uint8_t result_code = memory[0xA000];

		constexpr std::array<std::uint8_t, 3> expected_result_header { 0xde, 0xb0, 0x61 };
		const std::array result_header { memory[0xA001], memory[0xA002], memory[0xA003] };

		const std::string result = decode_result_from_memory(0xA004, 0xBFFF, memory);

		if (expected_result_header != result_header)
		{
			const auto image_path = require_success(export_lcd(*engine, rom_file_path));

			FAIL(
				std::format("Unexpected header. Expected: {}. Got: {}. Error message: {}. Generated result image at: {}",
					expected_result_header,
					result_header,
					result,
					image_path.string()));
		}

		if (result_code != 0)
		{
			const auto image_path = require_success(export_lcd(*engine, rom_file_path));

			FAIL(
				std::format("Unexpected result code. Expected: 0. Got: {}. Error message: {}. Generated result image at: {}",
					result_code,
					result,
					image_path.string()));
		}

		if (expected_output != result)
		{
			const auto image_path = require_success(export_lcd(*engine, rom_file_path));

			FAIL(
				std::format("Unexpected result output. Expected: {}. Got: {}. Generated result image at: {}",
					expected_output,
					result,
					image_path.string()));
		}

		std::cout << result;
	}

	export void run_lcd_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output_path,
		const size_t num_t_cycles)
	{
		test_serial serial {};
		test_audio_sink audio_sink {};

		auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
		auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));

		engine->tick(num_t_cycles);

		using namespace stb;
		const stb_result<image> reference_img = load_image(expected_output_path, graphics::num_color_channels);
		REQUIRE_MESSAGE(reference_img.has_value(), std::format("Could not read reference image. {}", reference_img.error()));

		const auto actual_data = engine->lcd();
		const auto expected_data = reference_img.value().as_span();
		const bool generated_expected_output = std::ranges::equal(expected_data, actual_data);

		if (!generated_expected_output)
		{
			const auto image_path = require_success(export_lcd(*engine, rom_file_path));
			FAIL(std::format("Incorrect lcd result generated. Generated result image at {}", image_path.string()));
		}
	}
}
