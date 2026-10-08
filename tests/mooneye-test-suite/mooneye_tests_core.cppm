module;
#include "doctest.h"

export module mooneye;

import std;
import engine;
import cartridge;
import tests.core;

namespace mooneye
{
	export void run_test(const std::string_view rom_file_path)
	{
		using namespace tests;
		using namespace engine;
		using namespace cartridge;

		memory_lcd lcd {};
		dummy_audio audio {};
		memory_serial serial {};

		auto cartridge = required(load_rom_file(rom_file_path));
		auto engine = required(create_engine(cartridge, lcd, audio, serial));

		// Note: all tests in mooneye' suite are configured to last at max 120 emulated seconds
		constexpr size_t max_num_seconds = 120;
		constexpr size_t expected_num_result_numbers = 6;

		using result_sequence_t = std::array<std::uint8_t, expected_num_result_numbers>;
		result_sequence_t result {};

		for (size_t i = 0; i < max_num_seconds; i++)
		{
			constexpr size_t ticks_per_second = 4.19e6;
			engine->tick(ticks_per_second);

			if (serial.result().size() >= expected_num_result_numbers)
			{
				break;
			}
		}

		constexpr result_sequence_t expected_success_result { 3, 5, 8, 13, 21, 34 };
		const bool received_expected_result = std::ranges::equal(serial.result(), expected_success_result);

		if (!received_expected_result)
		{
			const auto image_path = required(save_lcd_screenshot(lcd.last_frame()));
			FAIL(std::format("Incorrect result sequence received: {}\nGenerated result image at {}", result, image_path.string()));
		}
	}

}