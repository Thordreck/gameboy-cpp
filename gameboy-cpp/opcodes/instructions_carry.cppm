
export module opcodes:carry;

export import cpu;
export import std;

import :common;

namespace opcodes
{
	export struct ccf
	{
		static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

		static void execute(cpu::cpu_state& cpu, const step_t)
		{
			cpu.reg.n_flag() = false;
			cpu.reg.h_flag() = false;
			cpu.reg.c_flag() = !cpu.reg.c_flag();
		}
	};

	export struct scf
	{
		static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

		static void execute(cpu::cpu_state& cpu, const step_t)
		{
			cpu.reg.n_flag() = false;
			cpu.reg.h_flag() = false;
			cpu.reg.c_flag() = true;
		}
	};
}
