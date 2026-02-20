export module opcodes:execute;

import cpu;
import std;
import :table;

namespace opcodes
{
	export template<typename T>
	concept InstructionExecutor = requires(cpu::cpu & cpu, instruction_fn_t instruction)
	{
		{ T::execute(cpu, instruction) } -> std::same_as<void>;
	};

	export struct default_instruction_executor
	{
		static void execute(cpu::cpu& cpu, instruction_fn_t instruction)
		{
			instruction(cpu);
		}
	};

} // namespace opcode
