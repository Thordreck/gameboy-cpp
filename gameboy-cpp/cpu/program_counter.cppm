
export module cpu:program_counter;
import std;

namespace cpu
{
	export class program_counter
	{
	public:
		using type_t = std::uint16_t;

		operator type_t() const { return value(); }

		type_t value() const { return value_;  }

		program_counter& operator=(const type_t new_value)
		{
			value_ = new_value;
			return *this;
		}

		program_counter& operator=(const program_counter other)
		{
			value_ = other;
			return *this;
		}

		program_counter& operator++()
		{
			value_++;
			return *this;
		}

		program_counter operator++(int)
		{
			program_counter temp = *this;
			++(*this);
			return temp;
		}

		program_counter& operator+=(const program_counter rhs)
		{
			value_ += rhs;
			return *this;
		}

		program_counter& operator+=(const type_t rhs)
		{
			value_ += rhs;
			return *this;
		}

		program_counter operator+(const int rhs) const
		{
			program_counter temp = *this;
			temp += rhs;
			return temp;
		}

		program_counter operator+(const type_t rhs) const
		{
			program_counter temp = *this;
			temp += rhs;
			return temp;
		}

		program_counter operator+(const program_counter rhs) const
		{
			program_counter temp = *this;
			temp += rhs;
			return temp;
		}

		auto operator<=>(const program_counter&) const = default;

	private:
		std::uint16_t value_{};
	};
}
