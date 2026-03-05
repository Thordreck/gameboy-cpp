
export module interrupts:enable;

import cpu;
import :common;

namespace interrupts
{
	export template<InterruptDescriptor interrupt>
	void enable(cpu::cpu& cpu)
	{
		const auto ie_value = cpu.memory().read(ie_address);
		cpu.memory().write(ie_address, ie_value | interrupt::ie_flag);
	}

	export template<InterruptDescriptor interrupt>
	void disable(cpu::cpu& cpu)
	{
		const auto ie_value = cpu.memory().read(ie_address);
		cpu.memory().write(ie_address, ie_value & ~interrupt::ie_flag);
	}

	export template<InterruptDescriptor interrupt>
	bool is_enabled(const cpu::cpu& cpu)
	{
		return (cpu.memory().read(ie_address) & interrupt::ie_flag) == interrupt::ie_flag;
	}
}