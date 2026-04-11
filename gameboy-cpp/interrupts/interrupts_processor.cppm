export module interrupts:processor;

export import cpu;
export import :service;

namespace interrupts
{
	export void process(cpu::cpu_state& cpu)
	{
		if (cpu.ime.enabled && is_any_interrupt_pending(cpu))
		{
			service_first_pending_interrupt(cpu);
		}
	}
}
