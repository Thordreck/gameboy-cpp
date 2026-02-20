export module opcodes:interrupt;

import cpu;
import std;

namespace opcodes
{
	export struct ei
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.ime_flag().request();
			cpu.pc()++;
		}
	};

	export struct di
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.ime_flag().disable();
			cpu.pc()++;
		}
	};
}