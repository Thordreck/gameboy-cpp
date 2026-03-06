
export module utilities:arithmetic;
export import std;

namespace utils
{
	export template<typename T>
	concept ArithmeticOperand
		= std::same_as<T, std::uint8_t>
		|| std::same_as<T, std::uint16_t>;
	
	export template<ArithmeticOperand T>
	bool constexpr check_add_overflow(const T lhs, const T rhs)
	{
		return static_cast<T>(lhs + rhs) < lhs;
	}

	export template<ArithmeticOperand T>
	bool constexpr check_half_add_overflow(const T lhs, const T rhs)
	{
		constexpr int half_bits = sizeof(T) == 1 ? 4 : 12;
		constexpr T mask = (static_cast<T>(1) << half_bits) - 1;

		return ((lhs & mask) + (rhs & mask)) > mask;
	}

	export template<ArithmeticOperand T>
	bool constexpr check_substract_underflow(const T lhs, const T rhs)
	{
		return lhs < rhs;
	}

	export template<ArithmeticOperand T>
	bool constexpr check_half_substract_underflow(const T lhs, const T rhs)
	{
		constexpr int half_bits = sizeof(T) == 1 ? 4 : 12;
		constexpr T mask = (static_cast<T>(1) << half_bits) - 1;

		return (lhs & mask) < (rhs & mask);
	}
}
