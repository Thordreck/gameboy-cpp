export module opcodes:pipeline;

import std;
import cpu;

import :fetch;
import :decode;
import :execute;

namespace opcodes
{
	template <typename T>
	concept InstructionsPipeline = requires(cpu::cpu & cpu)
	{
		{ T::step(cpu) } -> std::same_as<void>;
	};

	export template <OpcodeFetcher fetcher, OpcodeDecoder decoder, InstructionExecutor executor>
	struct instructions_pipeline
	{
		static void step(cpu::cpu& cpu)
		{
			const std::uint8_t opcode = fetcher::fetch(cpu);
			const instruction_fn_t instruction = decoder::decode(cpu, opcode);

			executor::execute(cpu, instruction);
		}
	};

	export using default_instructions_pipeline = instructions_pipeline<
		default_opcode_fetcher,
		default_opcode_decoder,
		default_instruction_executor>;
}
