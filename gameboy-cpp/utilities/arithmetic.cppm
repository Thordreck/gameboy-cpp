
export module utilities:arithmetic;
import std;

namespace utils
{
	export template <typename T>
	concept IntegralConvertible 
		= std::convertible_to<T, std::intmax_t> 
		|| std::convertible_to<T, std::uintmax_t>;
	
	export template<IntegralConvertible T>
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

	export template<IntegralConvertible Lhs, IntegralConvertible Rhs>
	bool constexpr check_half_add_overflow(const Lhs lhs, const Rhs rhs)
	{
		return (lhs & 0xF) + (rhs & 0xF) > 0xF;
	}

	export template<IntegralConvertible Lhs, IntegralConvertible Rhs>
	bool constexpr check_substract_underflow(const Lhs lhs, const Rhs rhs)
	{
		return rhs > lhs;
	}

	export template<IntegralConvertible Lhs, IntegralConvertible Rhs>
	bool constexpr check_half_substract_underflow(const Lhs lhs, const Rhs rhs)
	{
		return (lhs & 0xF) < (rhs & 0xF);
	}
}
