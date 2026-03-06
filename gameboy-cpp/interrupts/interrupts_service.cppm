export module interrupts:service;

export import std;
export import cpu;

export import :common;
export import :enable;
export import :request;
export import :dispatch;

namespace interrupts
{
	template<InterruptDescriptor... Interrupts>
	std::optional<interrupt_dispatcher> service_first_pending_by_priority(cpu::cpu& cpu)
	{
		std::optional<interrupt_dispatcher> handler = std::nullopt;

		((handler = try_service<Interrupts>(cpu)) || ...);
		return handler;
	}

	template <InterruptDescriptor interrupt>
	std::optional<interrupt_dispatcher> try_service(cpu::cpu& cpu)
	{
		if (is_pending<interrupt>(cpu))
		{
			return interrupt_dispatcher
			{
				dispatcher<interrupt>::execute,
				dispatcher<interrupt>::num_cycles
			};
		}

		return std::nullopt;
	}

	export bool is_any_interrupt_pending(const cpu::cpu& cpu)
	{
		return (cpu.memory().read(ie_address) & ie_mask) & (cpu.memory().read(if_address) & if_mask);
	}

	export template <InterruptDescriptor interrupt>
	bool is_pending(const cpu::cpu& cpu)
	{
		return is_enabled<interrupt>(cpu.memory()) && is_requested<interrupt>(cpu.memory());
	}

	export std::optional<interrupt_dispatcher> service_first_pending_interrupt(cpu::cpu& cpu)
	{
		return service_first_pending_by_priority<
			vblank_interrupt,
			lcd_interrupt,
			timer_interrupt,
			serial_interrupt,
			joypad_interrupt>(cpu);
	}
}
