export module opcodes:interrupt;

import cpu;
import std;

namespace opcodes
{
	using namespace cpu::literals;

	export struct ei
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.ime_flag().request();
			cpu.pc()++;
		}
	};

	export struct di
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.ime_flag().disable();
			cpu.pc()++;
		}
	};
}