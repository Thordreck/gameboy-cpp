module;
#include "doctest.h"

export module mooneye;

import std;
import mbc;
import utilities;
import cartridge;
import emulator.core;
import emulator.engine;
import emulator.gameboy;

namespace mooneye
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
		[[nodiscard]] const std::vector<std::uint8_t>& input_data() const { return buffer; }

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

	export void run_test(const std::string_view rom_file_path)
	{
		test_serial serial {};
		test_audio_sink audio_sink {};

		auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
		auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));

		// results
		// Note: all tests in mooneye' suite are configured to last at max 120 emulated seconds
		constexpr size_t max_num_seconds = 120;
		constexpr size_t ticks_per_second = 4.19e6;
		constexpr size_t expected_num_result_numbers = 6;

		using result_sequence_t = std::array<std::uint8_t, expected_num_result_numbers>;
		result_sequence_t result {};

		for (size_t i = 0; i < max_num_seconds; i++)
		{
			engine->tick(ticks_per_second);

			if (serial.input_data().size() >= expected_num_result_numbers)
			{
				break;
			}
		}

		constexpr result_sequence_t expected_success_result { 3, 5, 8, 13, 21, 34 };

		REQUIRE_MESSAGE(
			std::ranges::equal(serial.input_data(), expected_success_result),
			std::format("Result sequence received: {}", result));
	}

}