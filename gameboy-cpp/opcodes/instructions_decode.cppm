
export module opcodes:decode;

import std;
import cpu;
import :table;

namespace opcodes
{
	export template <typename T>
	concept OpcodeDecoder = requires(const cpu::cpu & cpu, const std::uint8_t opcode)
	{
		{ T::decode(cpu, opcode) } -> std::convertible_to<instruction_fn_t>;
	};

	export template <auto table, auto prefixed_table>
	requires InstructionTable<table> && InstructionTable<prefixed_table>
	struct table_opcode_decoder
	{
		static instruction_fn_t decode(
			const cpu::cpu& cpu,
			const std::uint8_t opcode)
		{
			if (opcode != opcodes::prefix_opcode)
			{
				return table[opcode];
			}

			const std::uint8_t prefixed_opcode = cpu.memory()[cpu.pc() + 1];
			return prefixed_table[prefixed_opcode];
		}
	};

	export using default_opcode_decoder = table_opcode_decoder<
		default_instruction_table, 
		default_prefixed_instruction_table>;
}
