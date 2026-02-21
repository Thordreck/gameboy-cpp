
export module interrupts:request;

import cpu;
import :common;

namespace interrupts
{
	export template<InterruptDescriptor interrupt>
	void request(cpu::cpu& cpu)
	{
		cpu.memory()[if_address] |= interrupt::if_flag;
	}

	export template<InterruptDescriptor interrupt>
	void clear_request(cpu::cpu& cpu)
	{
		cpu.memory()[if_address] &= ~interrupt::if_flag;
	}

	export template<InterruptDescriptor interrupt>
	bool is_requested(const cpu::cpu& cpu)
	{
		return (cpu.memory()[if_address] & interrupt::if_flag) == interrupt::if_flag;
	}
}
