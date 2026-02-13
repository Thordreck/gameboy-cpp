
export module opcodes:table;

import std;
import cpu;
import :arithmetic;

namespace opcodes
{
	export using opcode_t = std::uint8_t;
	export using instruction_fn_t = void(*)(cpu::cpu&);

	export template<typename T>
	concept Instruction = requires(cpu::cpu & cpu)
	{
		{ T::execute(cpu) } -> std::same_as<void>;
	};

	export template<opcode_t Opcode, Instruction Instr>
	struct instruction_definition 
	{
		static constexpr opcode_t opcode = Opcode;
		static constexpr auto execute = &Instr::execute;
	};

	export template<typename T>
	concept InstructionDefinitionConcept = requires()
	{
		{ T::opcode } -> std::convertible_to<opcode_t>;
		{ T::execute } -> std::convertible_to<instruction_fn_t>;
	};

	export template<InstructionDefinitionConcept... Defs>
	struct instruction_table_builder
	{
		static constexpr auto build()
		{
			constexpr opcode_t size = get_max_opcode() + 1;
			std::array<instruction_fn_t, size> table{};

			((table[Defs::opcode] = Defs::execute), ...);
			return table;
		}

	private:
		static consteval opcode_t get_max_opcode()
		{
			opcode_t max = 0;
			((max = Defs::opcode > max ? Defs::opcode : max), ...);
			return max;
		}
	};

#ifdef __INTELLISENSE__
	#pragma diag_suppress 28
	#pragma diag_suppress 3160
#endif
	export using default_instruction_table_builder = instruction_table_builder<
		instruction_definition<0x80, add_a_b>,
		instruction_definition<0x81, add_a_c>,
		instruction_definition<0x82, add_a_d>,
		instruction_definition<0x83, add_a_e>,
		instruction_definition<0x84, add_a_h>,
		instruction_definition<0x85, add_a_l>
	>;
}