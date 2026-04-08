module;
#include "doctest.h"

export module blargg;

import emulator.core;
import utilities;
import std;

namespace
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

}

namespace blargg
{
	export void run_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output,
		const size_t max_num_machine_cycles)
	{
		// engine
		emulator::engine engine {};

		{
			const auto rom_data = require_success(utils::read_binary_file(rom_file_path));
			engine.load_rom(rom_data);
		}

		// results
		std::string result{};
		const size_t max_num_timer_cycles = max_num_machine_cycles * 4;
		memory::memory_bus bus = engine.memory_bus();

		for (size_t i = 0; i < max_num_timer_cycles; i++)
		{
			engine.tick();

			if (const auto character = read_io_result_output(bus); character.has_value())
			{
				result += character.value();
			}

			if (result == expected_output)
			{
				break;
			}
		}

		REQUIRE_EQ(expected_output, result);
		std::cout << result;
	}
}
