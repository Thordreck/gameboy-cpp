export module opcodes:misc;

import cpu;

namespace opcodes
{
	export struct nop
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc()++;
		}
	};
}
