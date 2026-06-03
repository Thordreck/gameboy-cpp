
export module timer:common;
import std;

namespace timer
{
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
