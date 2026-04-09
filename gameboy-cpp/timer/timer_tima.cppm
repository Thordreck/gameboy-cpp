
export module timer:tima;

export import std;
export import :common;
export import :tac;
import utilities;

namespace
{
	bool is_tac_clock_bit_set(const std::uint16_t value, const timer::tac_clock clock)
	{
		switch (clock)
		{
		case timer::tac_clock::_00: return utils::is_bit_set<9>(value);
		case timer::tac_clock::_01: return utils::is_bit_set<3>(value);
		case timer::tac_clock::_10: return utils::is_bit_set<5>(value);
		case timer::tac_clock::_11: return utils::is_bit_set<7>(value);
		default: std::unreachable();
		}
	}
}

namespace timer
{
	class tima
	{
	public:
		tima()
			: counter{ }
		{}

		std::uint8_t value() const { return counter; }
		operator std::uint8_t() const { return value(); }

		tima& operator=(const std::uint8_t new_value)
		{ 
			counter = new_value; 
			return *this;
		}

		bool tick(
			const std::uint16_t prev_div,
			const std::uint16_t current_div,
			const tac_clock selected_clock)
		{
			const bool prev_bit_set = is_tac_clock_bit_set(prev_div, selected_clock);
			const bool current_bit_set = is_tac_clock_bit_set(current_div, selected_clock);

			bool overflow = false;

			// Falling edge
			if (prev_bit_set && !current_bit_set)
			{
				overflow = utils::check_add_overflow<std::uint8_t>(counter, 1);
				counter++;
			}

			return overflow;
		}

	private:
		std::uint8_t counter;
	};
}
