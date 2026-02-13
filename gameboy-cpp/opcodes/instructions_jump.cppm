
export module opcodes:jump;

import cpu;
import utilities;
import std;

namespace opcodes
{
	export struct jp_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc = utils::read_two_byte_little_endian(cpu.memory, cpu.pc.as_bytes() + 1);
		}
	};

	template<auto T>
	concept CPUStateCondition = requires(const cpu::cpu& cpu)
	{
		{ T(cpu) } -> std::convertible_to<bool>;
	};

	template <auto Condition>
	requires CPUStateCondition<Condition>
	struct jp_cc_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc = Condition(cpu)
				? utils::read_two_byte_little_endian(cpu.memory, cpu.pc.as_bytes() + 1)
				: cpu.pc.as_bytes() + 3;
		}
	};

	inline bool is_z_set(const cpu::cpu& cpu) { return cpu.registers.z_flag(); }
	inline bool is_z_not_set(const cpu::cpu& cpu) { return !is_z_set(cpu); }
	inline bool is_c_set(const cpu::cpu& cpu) { return cpu.registers.c_flag(); }
	inline bool is_c_not_set(const cpu::cpu& cpu) { return !is_c_set(cpu); }

	export using jp_z_n16 = jp_cc_n16<is_z_set>;
	export using jp_nz_n16 = jp_cc_n16<is_z_not_set>;
	export using jp_c_n16 = jp_cc_n16<is_c_set>;
	export using jp_nc_n16 = jp_cc_n16<is_c_not_set>;
}