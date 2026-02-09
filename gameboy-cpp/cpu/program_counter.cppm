
export module cpu:program_counter;
export import std;

namespace cpu
{
	export class program_counter
	{
	public:
		using type_t = std::uint16_t;

		operator type_t() const { return value; }

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

	private:
		std::uint16_t value{};
	};
}
