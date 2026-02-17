
export module opcodes:jump;

import :common;
import cpu;
import utilities;
import std;

namespace opcodes
{
	export struct jp_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc() = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);
		}
	};

	template <CPUStateCondition condition>
	struct jp_cc_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc() = condition::evaluate(cpu)
				? utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1)
				: cpu.pc() + 3;
		}
	};

	export using jp_z_n16 = jp_cc_n16<is_z_set>;
	export using jp_nz_n16 = jp_cc_n16<is_z_not_set>;
	export using jp_c_n16 = jp_cc_n16<is_c_set>;
	export using jp_nc_n16 = jp_cc_n16<is_c_not_set>;
}