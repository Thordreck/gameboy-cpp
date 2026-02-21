export module interrupts:service;

import cpu;
import std;

import :common;
import :enable;
import :request;
import :dispatch;

namespace interrupts
{
	template<InterruptDescriptor... Interrupts>
	void service_first_pending_by_priority(cpu::cpu& cpu)
	{
		(try_service<Interrupts>(cpu) || ...);
	}

	template <InterruptDescriptor interrupt>
	bool try_service(cpu::cpu& cpu)
	{
		if (is_pending<interrupt>(cpu))
		{
			dispatch<interrupt>(cpu);
			return true;
		}

		return false;
	}

	export bool is_any_interrupt_pending(const cpu::cpu& cpu)
	{
		return (cpu.memory()[ie_address] & ie_mask) & (cpu.memory()[if_address] & if_mask);
	}

	export template <InterruptDescriptor interrupt>
	bool is_pending(const cpu::cpu& cpu)
	{
		return is_enabled<interrupt>(cpu) && is_requested<interrupt>(cpu);
	}

	export void service_first_pending_interrupt(cpu::cpu& cpu)
	{
		service_first_pending_by_priority<
			vblank_interrupt,
			lcd_interrupt,
			timer_interrupt,
			serial_interrupt,
			joypad_interrupt>(cpu);
	}
}
