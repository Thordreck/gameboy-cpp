export module opcodes:subroutine;

import std;
import cpu;
import :common;
import :jump;
import :stack;

namespace opcodes
{
	export struct call_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			push_stack(cpu, cpu.pc() + 3);
			jp_n16::execute(cpu);
		}
	};

	template<CPUStateCondition condition>
	struct call_cc_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			if (condition::evaluate(cpu))
			{
				call_n16::execute(cpu);
			}
			else 
			{
				cpu.pc() += 3;
			}
		}
	};

	export using call_z_n16 = call_cc_n16<is_z_set>;
	export using call_nz_n16 = call_cc_n16<is_z_not_set>;
	export using call_c_n16 = call_cc_n16<is_c_set>;
	export using call_nc_n16 = call_cc_n16<is_c_not_set>;

	export struct ret
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc() = pop_stack(cpu);
		}
	};

	template<CPUStateCondition condition>
	struct ret_cc
	{
		static void execute(cpu::cpu& cpu)
		{
			if (condition::evaluate(cpu))
			{
				ret::execute(cpu);
			}
			else 
			{
				cpu.pc()++;
			}
		}
	};

	export using ret_z = ret_cc<is_z_set>;
	export using ret_nz = ret_cc<is_z_not_set>;
	export using ret_c = ret_cc<is_c_set>;
	export using ret_nc = ret_cc<is_c_not_set>;

	export struct reti
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc() = pop_stack(cpu);
			// TODO: enable ime flag
		}
	};

	template<std::uint8_t vec>
	requires RSTVector<vec>
	struct rst_vec
	{
		static void execute(cpu::cpu& cpu)
		{
			push_stack(cpu, cpu.pc() + 1);
			cpu.pc() = vec;
		}
	};

	export using rst_00 = rst_vec <0x00>;
	export using rst_08 = rst_vec <0x08>;
	export using rst_10 = rst_vec <0x10>;
	export using rst_18 = rst_vec <0x18>;
	export using rst_20 = rst_vec <0x20>;
	export using rst_28 = rst_vec <0x28>;
	export using rst_30 = rst_vec <0x30>;
	export using rst_38 = rst_vec <0x38>;
}