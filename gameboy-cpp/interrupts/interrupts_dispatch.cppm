export module interrupts:dispatch;

import cpu;
import utilities;
import :common;
import :request;

namespace interrupts
{
	void push_stack(cpu::cpu& cpu, const cpu::register_16::type_t value)
	{
		cpu.memory()[--cpu.sp()] = utils::most_significant_byte(value);
		cpu.memory()[--cpu.sp()] = utils::less_significant_byte(value);
	}

	export template <InterruptDescriptor interrupt>
	void dispatch(cpu::cpu& cpu)
	{
		cpu.ime_flag().disable();
		clear_request<interrupt>(cpu);
		
		push_stack(cpu, cpu.pc());
		cpu.pc() = interrupt::handler_address;
	}
}
