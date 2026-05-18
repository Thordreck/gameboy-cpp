module;
#include "doctest.h"

export module blargg;

import std;
import mbc;
import memory;
import utilities;
import cartridge;
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

	export void run_test(
		const std::string_view rom_file_path,
		const std::string_view expected_output,
		const size_t num_t_cycles)
	{
		test_serial serial {};

		auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
		auto engine = require_success(emulator::create_engine(cartridge, serial));

		engine->tick(num_t_cycles);
		const std::string result = serial.result();

		REQUIRE_EQ(expected_output, result);
		std::cout << result;
	}
}
