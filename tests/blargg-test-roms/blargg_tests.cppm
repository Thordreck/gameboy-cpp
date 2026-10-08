module;
#include "doctest.h"

export module blargg;

import std;
import memory;
import engine;
import cartridge;
import tests.core;

namespace blargg
{
	export void run_serial_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output,
		const size_t num_t_cycles)
	{
		using namespace tests;
		using namespace engine;
		using namespace cartridge;

		memory_lcd lcd {};
		dummy_audio audio {};
		memory_serial serial {};

		auto cartridge = required(load_rom_file(rom_file_path));
		auto engine = required(create_engine(cartridge, lcd, audio, serial));

		engine->tick(num_t_cycles);

		const std::string result = serial.result();
		const bool received_expected_result = result == expected_output;

		if (!received_expected_result)
		{
			const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));
			FAIL(std::format("Incorrect result received: {}\nGenerated result image at {}", result, image_path.string()));
		}

		std::cout << result;
	}

	export void run_memory_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output,
		const size_t num_t_cycles)
	{
		using namespace tests;
		using namespace engine;
		using namespace cartridge;

		memory_lcd lcd {};
		dummy_audio audio {};
		dummy_serial serial {};

		auto cartridge = required(load_rom_file(rom_file_path));
		auto engine = required(create_engine(cartridge, lcd, audio, serial));

		engine->tick(num_t_cycles);

		const auto memory = engine->memory();
		const std::uint8_t result_code = memory[0xA000];

		constexpr std::array<std::uint8_t, 3> expected_result_header { 0xde, 0xb0, 0x61 };
		const std::array result_header { memory[0xA001], memory[0xA002], memory[0xA003] };
		const std::string result = read_memory_as_string(0xA004, 0xBFFF, memory);

		if (expected_result_header != result_header)
		{
			const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));

			FAIL(
				std::format("Unexpected header. Expected: {}. Got: {}. Error message: {}. Generated result image at: {}",
					expected_result_header,
					result_header,
					result,
					image_path.string()));
		}

		if (result_code != 0)
		{
			const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));

			FAIL(
				std::format("Unexpected result code. Expected: 0. Got: {}. Error message: {}. Generated result image at: {}",
					result_code,
					result,
					image_path.string()));
		}

		if (expected_output != result)
		{
			const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));

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
		using namespace tests;
		using namespace engine;
		using namespace cartridge;

		memory_lcd lcd {};
		dummy_audio audio {};
		dummy_serial serial {};

		auto cartridge = required(load_rom_file(rom_file_path));
		auto engine = required(create_engine(cartridge, lcd, audio, serial));

		engine->tick(num_t_cycles);

		const bool generated_expected_output = required(compare_lcd_with_reference(lcd.last_frame(), expected_output_path));

		if (!generated_expected_output)
		{
			const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));
			FAIL(std::format("Incorrect lcd result generated. Generated result image at {}", image_path.string()));
		}
	}

}
