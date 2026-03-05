
export module interrupts:request;

import cpu;
import :common;

namespace interrupts
{
	export template<InterruptDescriptor interrupt>
	void request(cpu::cpu& cpu)
	{
		const auto value = cpu.memory().read(if_address);
		cpu.memory().write(if_address, value | interrupt::if_flag);
	}

	export template<InterruptDescriptor interrupt>
	void clear_request(cpu::cpu& cpu)
	{
		const auto value = cpu.memory().read(if_address);
		cpu.memory().write(if_address, value & ~interrupt::if_flag);
	}

	export template<InterruptDescriptor interrupt>
	bool is_requested(const cpu::cpu& cpu)
	{
		return (cpu.memory().read(if_address) & interrupt::if_flag) == interrupt::if_flag;
	}
}
