
export module cpu:program_counter;
export import std;

namespace cpu
{
	export class program_counter
	{
	public:
		using type_t = std::uint16_t;

		explicit operator type_t() const { return value; }
		type_t as_bytes() const { return value; }

		program_counter& operator=(const type_t new_value)
		{
			value = new_value;
			return *this;
		}

		program_counter& operator++()
		{
			value++;
			return *this;
		}

		program_counter operator++(int)
		{
			program_counter temp = *this;
			++(*this);
			return temp;
		}

		program_counter& operator+=(const type_t rhs)
		{
			value += rhs;
			return *this;
		}

		program_counter operator+(const type_t rhs) const
		{
			program_counter temp = *this;
			temp += rhs;
			return temp;
		}

		auto operator<=>(const program_counter&) const = default;

	private:
		std::uint16_t value{};
	};
}
