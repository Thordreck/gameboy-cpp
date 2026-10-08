module;
#include "doctest.h"

export module gbmicro;

import std;
import memory;
import engine;
import cartridge;
import tests.core;

namespace gbmicro
{
	[[nodiscard]] bool has_test_succeeded(memory::ReadOnlyMemory auto const& memory)
	{
		constexpr std::uint16_t test_result_address { 0xFF82 };
		constexpr std::uint8_t test_passed_value { 0x01 };

		return memory.read(test_result_address) == test_passed_value;
	}

	[[nodiscard]] bool has_test_failed(memory::ReadOnlyMemory auto const& memory)
	{
		constexpr std::uint16_t test_result_address { 0xFF82 };
		constexpr std::uint8_t test_failed_value { 0xFF };

		return memory.read(test_result_address) == test_failed_value;
	}

	[[nodiscard]] bool has_test_completed(memory::ReadOnlyMemory auto const& memory)
	{
		return has_test_succeeded(memory) || has_test_failed(memory);
	}

	[[nodiscard]] std::uint8_t expected_result(memory::ReadOnlyMemory auto const& memory)
	{
		constexpr std::uint16_t expected_result_address { 0xFF81 };
		return memory.read(expected_result_address);
	}

	[[nodiscard]] std::uint8_t actual_result(memory::ReadOnlyMemory auto const& memory)
	{
		constexpr std::uint16_t actual_result_address { 0xFF80 };
		return memory.read(actual_result_address);
	}

	export void run_test(const std::string_view rom_file_path)
	{
		using namespace tests;
		using namespace engine;
		using namespace cartridge;

		dummy_lcd lcd {};
		dummy_audio audio {};
		dummy_serial serial {};

		auto cartridge = required(load_rom_file(rom_file_path));
		auto engine = required(create_engine(cartridge, lcd, audio, serial));
		const auto memory = engine->memory();

		while (!has_test_completed(memory))
		{
			engine->tick(4);
		}

		REQUIRE(has_test_succeeded(memory));
		REQUIRE_EQ(expected_result(memory), actual_result(memory));
	}

}