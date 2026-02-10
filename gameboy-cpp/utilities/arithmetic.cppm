
export module utilities:arithmetic;
export import std;

namespace utils
{
	export template<typename T>
	requires std::integral<T> || std::floating_point<T> || std::signed_integral<T>
	bool constexpr check_add_overflow(const T lhs, const T rhs)
	{
		if (rhs > 0 && lhs > std::numeric_limits<T>::max() - rhs)
		{
			return true;
		}

		if (rhs < 0 && lhs < std::numeric_limits<T>::min() - rhs)
		{
			return true;
		}

		return false;
	}

	export bool constexpr check_half_add_overflow(const std::uint8_t lhs, const std::uint8_t rhs)
	{
		return (lhs & 0xF) + (rhs & 0xF) > 0xF;
	}
}
