export module utilities:bits;
import std;

namespace utils
{
	export template<std::unsigned_integral T>
	requires (sizeof(T) == 2)
	constexpr std::uint8_t most_significant_byte(const T input)
	{
		return static_cast<uint8_t>((input >> 8) & 0xFF);
	}

	export template<std::unsigned_integral T>
	requires (sizeof(T) == 2)
	constexpr std::uint8_t less_significant_byte(const T input)
	{
		return static_cast<uint8_t>(input & 0xFF);
	}

	export template<auto BitIndex, std::unsigned_integral T>
	requires std::unsigned_integral<decltype(BitIndex)> && (BitIndex < sizeof(T) * 8)
	constexpr bool is_bit_set(const T input)
	{
		return input & (T{1} << BitIndex);
	}
}
