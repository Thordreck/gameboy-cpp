
export module timer:tac;
export import memory;

import std;
import utilities;
import :common;

namespace timer
{
	export bool is_tac_enabled(const memory::memory_span_t memory)
	{
		return utils::is_bit_set<tac_enable_bit>(memory[tac_address]);
	}
	
	export void enable_tac(memory::memory_span_t memory)
	{
		utils::set_bit<tac_enable_bit>(memory[tac_address]);
	}

	export void disable_tac(memory::memory_span_t memory)
	{
		utils::unset_bit<tac_enable_bit>(memory[tac_address]);
	}

	export std::uint8_t to_bit_index(const tac_clock clock)
	{
		return std::to_underlying(clock);
	}

	export std::optional<tac_clock> from_bit_index(const std::uint8_t bit)
	{
		switch (bit)
		{
		case 1: return tac_clock::_01;
		case 3: return tac_clock::_10;
		case 5: return tac_clock::_11;
		case 7: return tac_clock::_00;
		default: return std::nullopt;
		}
	}

	export tac_clock get_tac_clock(const memory::memory_span_t memory)
	{
		const std::uint8_t raw_clock = memory[tac_address] & tac_clock_select_mask;
		return from_bit_index(raw_clock).value_or(tac_clock::_00);
	}

	export bool is_tac_clock_bit_set(const std::uint16_t value, const tac_clock clock)
	{
		switch (clock)
		{
		case tac_clock::_00: return utils::is_bit_set<7>(value);
		case tac_clock::_01: return utils::is_bit_set<1>(value);
		case tac_clock::_10: return utils::is_bit_set<3>(value);
		case tac_clock::_11: return utils::is_bit_set<5>(value);
		default: std::unreachable();
		}
	}
}
