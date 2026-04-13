export module interrupts:processor;

export import cpu;
export import :service;
import memory;

namespace interrupts
{
	export template <memory::ReadOnlyMemory Memory>
	void process(cpu::cpu_state& cpu, const Memory& memory)
	{
		if (cpu.ime.enabled && is_any_interrupt_pending(memory))
		{
			service_first_pending_interrupt(memory);
		}
	}
}
