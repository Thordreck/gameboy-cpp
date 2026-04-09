
export module timer:common;

export import std;

namespace timer
{
	export constexpr std::uint16_t div_address = 0xFF04;
	export constexpr std::uint16_t tima_address = 0xFF05;
	export constexpr std::uint16_t tma_address = 0xFF06;
	export constexpr std::uint16_t tac_address = 0xFF07;

	export constexpr std::uint8_t tac_enable_bit = 2;
	export constexpr std::uint8_t tac_clock_select_mask = 0b11;

	export enum class tac_clock : std::uint8_t
	{
		_01 = 3,
		_10 = 5,
		_11 = 7,
		_00 = 9,
	};

	export constexpr auto get_tack_clock_bit_index(const tac_clock clock)
	{
		return std::to_underlying(clock);
	}
}
