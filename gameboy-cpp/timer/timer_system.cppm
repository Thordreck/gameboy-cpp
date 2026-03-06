
export module timer:timer_system;

export import std;
export import memory;
export import interrupts;

export import :div;
export import :tima;
export import :tma;
export import :tac;

namespace timer
{
	export class timer_system
	{
	public:
		timer_system()
			: divider_register{}
			, timer_counter{ }
			, timer_modulo{ }
			, timer_control{}
			, overflow_detected{ false }
			, ticks_until_interrupt{}
			, memory{ nullptr }
		{}

		div& divider() { return divider_register; }
		const div& divider() const { return divider_register; }

		tima& counter() { return timer_counter; }
		const tima& counter() const { return timer_counter; }

		tma& modulo() { return timer_modulo; }
		const tma& modulo() const { return timer_modulo; }

		tac& control() { return timer_control; }
		const tac& control() const { return timer_control; }

		void tick()
		{
			if (overflow_detected && --ticks_until_interrupt <= 0)
			{
				timer_counter = timer_modulo.value;
				overflow_detected = false;

				using namespace interrupts;
				request<timer_interrupt>(*memory);
			}

			// Div
			const std::uint16_t prev_div = divider_register;
			divider_register.tick();
			const std::uint16_t current_div = divider_register;

			// Tima
			if (!overflow_detected && timer_control.enabled)
			{
				overflow_detected = timer_counter.tick(prev_div, current_div, timer_control.clock);
				ticks_until_interrupt = overflow_detected ? 4 : 0;
			}
		}

		void cancel_pending_interrupt()
		{
			overflow_detected = false;
			ticks_until_interrupt = 0;
		}

		void connect(memory::memory_bus& bus) { memory = &bus; }

	private:
		div divider_register;
		tima timer_counter;
		tma timer_modulo;
		tac timer_control;

		bool overflow_detected;
		std::uint8_t ticks_until_interrupt;

		memory::memory_bus* memory;
	};
}
