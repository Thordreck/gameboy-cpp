export module interrupts:dispatch;

export import std;
export import cpu;
export import utilities;
export import :common;
export import :request;

namespace interrupts
{
	export template <InterruptDescriptor interrupt>
	struct dispatcher
	{
		static constexpr cpu::machine_cycle num_cycles() 
		{ 
			using namespace cpu::literals;
			return 5_m_cycle;
		}

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<0>(cpu.cycle()))
			{
				cpu.ime_flag().disable();
				clear_request<interrupt>(cpu.memory());
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				cpu.memory().write(--cpu.sp(), utils::most_significant_byte(cpu.pc().value()));
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				cpu.memory().write(--cpu.sp(), utils::less_significant_byte(cpu.pc().value()));
			}
			else if (cpu::is_end_of_machine_cycle<4>(cpu.cycle()))
			{
				cpu.pc() = interrupt::handler_address;
			}
		}
	};
}
