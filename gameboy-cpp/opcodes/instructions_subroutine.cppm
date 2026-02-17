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
}