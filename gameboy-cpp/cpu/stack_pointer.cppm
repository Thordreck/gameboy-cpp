
export module cpu:stack_pointer;
import std;

namespace cpu
{
	export class stack_pointer
	{
	public:
		using type_t = std::uint16_t;

		operator type_t() const { return value(); }
		type_t value() const { return value_; }

		stack_pointer& operator=(const type_t new_value)
		{
			value_ = new_value;
			return *this;
		}

		stack_pointer& operator++()
		{
			value_++;
			return *this;
		}

		stack_pointer operator++(int)
		{
			stack_pointer temp = *this;
			++(*this);
			return temp;
		}

		stack_pointer& operator--()
		{
			value_--;
			return *this;
		}

		stack_pointer operator--(int)
		{
			stack_pointer temp = *this;
			--(*this);
			return temp;
		}

	private:
		std::uint16_t value_{};
	};
}