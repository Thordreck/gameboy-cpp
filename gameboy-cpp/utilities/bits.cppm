export module utilities:bits;
import std;

namespace utils
{
	export template<typename T>
	requires std::unsigned_integral<T> && (sizeof(T) == 2)
	constexpr std::uint8_t most_significant_byte(const T input)
	{
		return static_cast<uint8_t>((input >> 8) & 0xFF);
	}

	export template<typename T>
	requires std::unsigned_integral<T> && (sizeof(T) == 2)
	constexpr std::uint8_t less_significant_byte(const T input)
	{
		return static_cast<uint8_t>(input & 0xFF);
	}
}
