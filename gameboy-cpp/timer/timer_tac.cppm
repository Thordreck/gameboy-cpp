
export module timer:tac;

export import std;
export import :common;

import utilities;

namespace
{
	timer::tac_clock to_clock_select(const std::uint8_t value)
	{
		const std::uint8_t clock_value = value & timer::tac_clock_select_mask;

		switch (clock_value)
		{
		case 0b00: return timer::tac_clock::_00;
		case 0b01: return timer::tac_clock::_01;
		case 0b10: return timer::tac_clock::_10;
		case 0b11: return timer::tac_clock::_11;
		default: std::unreachable();
		}
	}

	std::uint8_t to_register_value(const timer::tac_clock clock_value)
	{
		switch (clock_value)
		{
		case timer::tac_clock::_00: return 0b00;
		case timer::tac_clock::_01: return 0b01;
		case timer::tac_clock::_10: return 0b10;
		case timer::tac_clock::_11: return 0b11;
		default: std::unreachable();
		}
	}
}

namespace timer
{
	export struct tac
	{
		bool enabled{ false };
		tac_clock clock{ tac_clock::_00 };

		tac& operator=(const std::uint8_t value)
		{
			enabled = utils::is_bit_set<tac_enable_bit>(value);
			clock = to_clock_select(value);
			
			return *this;
		}

		operator std::uint8_t() const 
		{
			return (enabled << tac_enable_bit) | to_register_value(clock);
		}
	};
}
