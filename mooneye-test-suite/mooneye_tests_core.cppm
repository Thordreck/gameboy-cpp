module;
#include "doctest.h"

export module mooneye;

import emulator.core;
import utilities;

namespace mooneye
{
	template<typename TExpected, typename TError>
	TExpected require_success(const std::expected<TExpected, TError>& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
		return result.value();
	}

	std::optional<std::uint8_t> read_io_result_output(memory::memory_bus& memory)
	{
		constexpr memory::memory_address_t sb = 0xFF01;
		constexpr memory::memory_address_t sc = 0xFF02;
		constexpr memory::memory_data_t transfer_start = 0x80;

		if (memory.read(sc) & transfer_start)
		{
			memory.write(sc, 0);
			return memory.read(sb);
		}

		return std::nullopt;
	}

	export void run_test(const std::string_view rom_file_path)
	{
		// engine
		emulator::engine engine {};

		{
			const auto rom_data = require_success(utils::read_binary_file(rom_file_path));
			engine.load_rom(rom_data);
		}

		// results
		// Note: all tests in mooneye' suite are configured to last at max 120 emulated seconds
		constexpr size_t max_num_timer_cycles = 127e6;
		constexpr size_t expected_num_result_numbers = 6;

		memory::memory_bus memory_bus = engine.memory_bus();
		size_t num_result_numbers_read { 0 };

		using result_sequence_t = std::array<std::uint8_t, expected_num_result_numbers>;
		result_sequence_t result {};

		for (size_t i = 0; i < max_num_timer_cycles; i++)
		{
			engine.tick();

			if (const auto read_result = read_io_result_output(memory_bus); read_result.has_value())
			{
				result[num_result_numbers_read++] = read_result.value();
			}

			if (num_result_numbers_read >= expected_num_result_numbers)
			{
				break;
			}
		}

		constexpr result_sequence_t expected_success_result { 3, 5, 8, 13, 21, 34 };

		REQUIRE_MESSAGE(
			std::ranges::equal(result, expected_success_result),
			std::format("Result sequence received: {}", result));
	}

}