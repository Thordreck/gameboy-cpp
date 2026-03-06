
export module opcodes:carry;

export import cpu;
export import std;

namespace opcodes
{
	using namespace cpu::literals;

	export struct ccf
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<0>(cpu.cycle()))
			{
				cpu.reg().n_flag() = false;
				cpu.reg().h_flag() = false;
				cpu.reg().c_flag() = !cpu.reg().c_flag();
			}
		}
	};

	export struct scf
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<0>(cpu.cycle()))
			{
				cpu.reg().n_flag() = false;
				cpu.reg().h_flag() = false;
				cpu.reg().c_flag() = true;
			}
		}
	};
}
