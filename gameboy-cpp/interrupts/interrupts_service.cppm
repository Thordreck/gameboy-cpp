export module interrupts:service;

export import std;
export import cpu;

export import :common;
export import :enable;
export import :request;
export import :dispatch;

namespace interrupts
{
	export template <InterruptDescriptor interrupt>
	bool is_pending(const cpu::cpu_state& cpu)
	{
		return is_enabled<interrupt>(*cpu.memory) && is_requested<interrupt>(*cpu.memory);
	}

	template <InterruptDescriptor interrupt>
	std::optional<interrupt_dispatcher> try_service(cpu::cpu_state& cpu)
	{
		if (is_pending<interrupt>(cpu))
		{
			return interrupt_dispatcher
			{
				dispatcher<interrupt>::execute,
				dispatcher<interrupt>::num_steps
			};
		}

		return std::nullopt;
	}

	template<InterruptDescriptor... Interrupts>
	std::optional<interrupt_dispatcher> service_first_pending_by_priority(cpu::cpu_state& cpu)
	{
		std::optional<interrupt_dispatcher> handler = std::nullopt;

		((handler = try_service<Interrupts>(cpu)) || ...);
		return handler;
	}

	export bool is_any_interrupt_pending(const cpu::cpu_state& cpu)
	{
		return (cpu.memory->read(ie_address) & ie_mask) & (cpu.memory->read(if_address) & if_mask);
	}

	export std::optional<interrupt_dispatcher> service_first_pending_interrupt(cpu::cpu_state& cpu)
	{
		return service_first_pending_by_priority<
			vblank_interrupt,
			lcd_interrupt,
			timer_interrupt,
			serial_interrupt,
			joypad_interrupt>(cpu);
	}
}
