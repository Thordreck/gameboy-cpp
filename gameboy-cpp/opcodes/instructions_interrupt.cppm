export module opcodes:interrupt;

export import cpu;
export import std;

import :common;

namespace opcodes
{
	export struct ei
	{
		static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

		static void execute(cpu::cpu_state& cpu, const step_t)
		{
			cpu.ime.requested = true;
		}
	};

	export struct di
	{
		static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

		static void execute(cpu::cpu_state& cpu, const step_t)
		{
			cpu.ime.requested = false;
			cpu.ime.enabled = false;
			cpu.ime.enabling = false;
		}
	};
}