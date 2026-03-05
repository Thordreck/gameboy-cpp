
export module timer:timer_system;
export import memory;

import std;
import :div;
import :tima;

namespace timer
{
	export class timer_system
	{
	public:
		timer_system(memory::memory_span_t memory)
			: div_timer{memory}
			, tima_timer{ memory }
			, memory{ memory }
		{}

		void tick()
		{
			// Increment div
			const std::uint16_t prev_div = div_timer++;
			const std::uint16_t current_div = div_timer;
		}

	private:
		div div_timer;
		tima tima_timer;

		memory::memory_span_t memory;
	};
}
