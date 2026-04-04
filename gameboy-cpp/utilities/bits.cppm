export module utilities:bits;
import std;

namespace utils
{
	export template<std::unsigned_integral T>
	requires (sizeof(T) == 2)
	constexpr std::uint8_t most_significant_byte(const T input)
	{
		return static_cast<std::uint8_t>((input >> 8) & 0xFF);
	}

	export template<std::unsigned_integral T>
	requires (sizeof(T) == 2)
	constexpr std::uint8_t less_significant_byte(const T input)
	{
		return static_cast<std::uint8_t>(input & 0xFF);
	}

	export template<auto BitIndex, std::unsigned_integral T>
	requires std::integral<decltype(BitIndex)> && (BitIndex < std::numeric_limits<T>::digits)
	constexpr bool is_bit_set(const T input)
	{
		return input & (T{1} << BitIndex);
	}

	export template<auto BitIndex, std::unsigned_integral T>
	requires std::integral<decltype(BitIndex)> && (BitIndex < std::numeric_limits<T>::digits) && (BitIndex >= 0)
	constexpr void set_bit(T& input)
	{
		input |= (1 << BitIndex);
	}

	export template<auto BitIndex, std::unsigned_integral T>
	requires std::integral<decltype(BitIndex)> && (BitIndex < std::numeric_limits<T>::digits) && (BitIndex >= 0)
	constexpr void unset_bit(T& input)
	{
		input |= ~(1 << BitIndex);
	}

	export template<auto BitIndex, std::unsigned_integral T>
	requires std::integral<decltype(BitIndex)> && (BitIndex < std::numeric_limits<T>::digits)
	constexpr bool bit_falling_edge(const T previous, const T current)
	{
		return is_bit_set<BitIndex>(previous) && !is_bit_set<BitIndex>(current);
	}

}
