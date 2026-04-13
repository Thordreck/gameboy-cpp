module;
#include "doctest.h"

export module blargg;

import emulator.core;
import utilities;
import std;

namespace blargg
{
	template<typename TExpected, typename TError>
	TExpected require_success(const std::expected<TExpected, TError>& result)
	{
		REQUIRE_MESSAGE(result.has_value(), std::format("Unexpected error. {}", result.error()));
		return result.value();
	}

	template<memory::Memory Memory>
	std::optional<std::uint8_t> read_io_result_output(Memory& memory)
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

		for (size_t t_cycle = 0; t_cycle < max_num_machine_cycles; t_cycle++)
		{
			engine.tick(4);

			if (const auto character = read_io_result_output(engine.memory()); character.has_value())
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
