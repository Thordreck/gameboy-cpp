export module opcodes:interrupt;

export import cpu;
export import std;

namespace opcodes
{
	using namespace cpu::literals;

	export struct ei
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<0>(cpu.cycle()))
			{
				cpu.ime_flag().request();
			}
		}
	};

	export struct di
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<0>(cpu.cycle()))
			{
				cpu.ime_flag().disable();
			}
		}
	};
}