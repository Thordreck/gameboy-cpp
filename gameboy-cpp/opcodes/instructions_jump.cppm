
export module opcodes:jump;

import cpu;
import utilities;

namespace opcodes
{
	export struct jp_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc = utils::read_two_byte_little_endian(cpu.memory, cpu.pc.as_bytes() + 1);
		}
	};
}