
export module opcodes:carry;

import cpu;
import std;

namespace opcodes
{
	export struct ccf
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().n_flag() = false;
			cpu.reg().h_flag() = false;
			cpu.reg().c_flag() = !cpu.reg().c_flag();

			cpu.pc()++;
		}
	};

	export struct scf
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().n_flag() = false;
			cpu.reg().h_flag() = false;
			cpu.reg().c_flag() = true;

			cpu.pc()++;
		}
	};
}
