module;
#include "doctest.h"

export module gbmicro;

import std;
import mbc;
import memory;
import graphics;
import utilities;
import cartridge;
import emulator.core;
import emulator.engine;
import emulator.gameboy;

namespace gbmicro
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
		[[nodiscard]] std::uint8_t transfer_bit(const std::uint8_t bit) { return 0x00; }
	};

	class test_audio_sink
	{
	public:
		static [[nodiscard]] std::uint8_t channel_count() { return 2; }
		static [[nodiscard]] std::uint32_t sample_rate() { return 44100; }
		static void write(const std::span<const float>) {}
	};

	template<memory::ReadOnlyMemory Memory>
	[[nodiscard]] bool has_test_succeeded(const Memory& memory)
	{
		constexpr std::uint16_t test_result_address { 0xFF82 };
		constexpr std::uint8_t test_passed_value { 0x01 };

		return memory.read(test_result_address) == test_passed_value;
	}

	template<memory::ReadOnlyMemory Memory>
	[[nodiscard]] bool has_test_failed(const Memory& memory)
	{
		constexpr std::uint16_t test_result_address { 0xFF82 };
		constexpr std::uint8_t test_failed_value { 0xFF };

		return memory.read(test_result_address) == test_failed_value;
	}

	template<memory::ReadOnlyMemory Memory>
	[[nodiscard]] bool has_test_completed(const Memory& memory)
	{
		return has_test_succeeded(memory) || has_test_failed(memory);
	}

	template<memory::ReadOnlyMemory Memory>
	[[nodiscard]] std::uint8_t expected_result(const Memory& memory)
	{
		constexpr std::uint16_t expected_result_address { 0xFF81 };
		return memory.read(expected_result_address);
	}

	template<memory::ReadOnlyMemory Memory>
	[[nodiscard]] std::uint8_t actual_result(const Memory& memory)
	{
		constexpr std::uint16_t actual_result_address { 0xFF80 };
		return memory.read(actual_result_address);
	}

	export void run_test(const std::string_view rom_file_path)
	{
		test_serial serial {};
		test_audio_sink audio_sink {};

		auto cartridge = require_success(cartridge::load_rom_file(rom_file_path));
		auto engine = require_success(emulator::create_engine(cartridge, audio_sink, serial));
		const auto memory = engine->memory();

		while (!has_test_completed(memory))
		{
			engine->tick(4);
		}

		REQUIRE(has_test_succeeded(memory));
		REQUIRE_EQ(expected_result(memory), actual_result(memory));
	}

}