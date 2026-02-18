export module opcodes:interrupt;

import cpu;
import std;

namespace opcodes
{
	export struct di
	{
		static void execute(cpu::cpu& cpu)
		{
			// TODO: implement ime flag in cpu
			cpu.pc()++;
		}
	};
}