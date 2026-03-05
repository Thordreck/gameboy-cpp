
export module timer:tima;
export import memory;

import :common;
import :tac;
import utilities;

namespace timer
{
	class tima
	{
	public:
		tima(memory::memory_span_t memory)
			: memory{ memory }
		{}

		memory::memory_data_t value() const { return memory[tima_address]; }

		void tick(
			const std::uint16_t prev_div,
			const std::uint16_t current_div,
			const tac_clock selected_clock)
		{
			const bool prev_bit_set = is_tac_clock_bit_set(prev_div, selected_clock);
			const bool current_bit_set = is_tac_clock_bit_set(current_div, selected_clock);

			// Falling edge
			if (prev_bit_set && !current_bit_set)
			{
				memory[tima_address]++;
			}
		}

	private:
		memory::memory_span_t memory;
	};
}
