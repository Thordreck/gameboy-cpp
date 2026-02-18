
export module opcodes:table;

import std;
import cpu;

import :bit;
import :misc;
import :jump;
import :load;
import :stack;
import :interrupt;
import :arithmetic;
import :subroutine;

namespace opcodes
{
	export using opcode_t = std::uint8_t;
	export using instruction_fn_t = void(*)(cpu::cpu&);
	export using instruction_table = std::array<instruction_fn_t, 256>;

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
			instruction_table table{};
			((table[Defs::opcode] = Defs::execute), ...);
			return table;
		}

	private:
		static consteval bool has_duplicate_opcodes()
		{
			constexpr opcode_t opcodes[] = {Defs::opcode...};

			for (std::size_t i = 0; i < sizeof...(Defs); ++i)
			{
				for (std::size_t j = i + 1; j < sizeof...(Defs); ++j)
				{
					if (opcodes[i] == opcodes[j])
						return true;
				}
			}

			return false;
		}

		static_assert(!has_duplicate_opcodes(), "Duplicate opcode detected in instruction_table_builder");
	};

	export constexpr opcode_t prefix_opcode = 0xCB;

#ifdef __INTELLISENSE__
#pragma diag_suppress 28
#pragma diag_suppress 3160
#endif
	export using default_instruction_table_builder = instruction_table_builder<

		instruction_definition<0x00, nop>,
		instruction_definition<0x01, ld_bc_n16>,
		instruction_definition<0x02, ld_bc_a>,
		instruction_definition<0x03, inc_bc>,
		instruction_definition<0x04, inc_b>,
		instruction_definition<0x05, dec_b>,
		instruction_definition<0x06, ld_b_n8>,
		instruction_definition<0x09, add_hl_bc>,
		instruction_definition<0x0A, ld_a_bc>,
		instruction_definition<0x0C, inc_c>,
		instruction_definition<0x0D, dec_c>,
		instruction_definition<0x0E, ld_c_n8>,
		instruction_definition<0x11, ld_de_n16>,
		instruction_definition<0x12, ld_de_a>,
		instruction_definition<0x13, inc_de>,
		instruction_definition<0x14, inc_d>,
		instruction_definition<0x15, dec_d>,
		instruction_definition<0x18, jr_n16>,
		instruction_definition<0x19, add_hl_de>,
		instruction_definition<0x1A, ld_a_de>,
		instruction_definition<0x1C, inc_e>,
		instruction_definition<0x1D, dec_e>,
		instruction_definition<0x1F, rra>,
		instruction_definition<0x20, jr_nz_n16>,
		instruction_definition<0x21, ld_hl_n16>,
		instruction_definition<0x22, ld_hli_a>,
		instruction_definition<0x23, inc_hl>,
		instruction_definition<0x24, inc_h>,
		instruction_definition<0x25, dec_h>,
		instruction_definition<0x26, ld_h_n8>,
		instruction_definition<0x28, jr_z_n16>,
		instruction_definition<0x29, add_hl_hl>,
		instruction_definition<0x2C, inc_l>,
		instruction_definition<0x2D, dec_l>,
		instruction_definition<0x2E, ld_l_n8>,
		instruction_definition<0x2A, ld_a_hli>,
		instruction_definition<0x30, jr_nc_n16>,
		instruction_definition<0x31, ld_sp_n16>,
		instruction_definition<0x32, ld_hld_a>,
		instruction_definition<0x35, dec_hl>,
		instruction_definition<0x39, add_hl_sp>,
		instruction_definition<0x3A, ld_a_hld>,
		instruction_definition<0x38, jr_c_n16>,
		instruction_definition<0x3C, inc_a>,
		instruction_definition<0x3D, dec_a>,
		instruction_definition<0x3E, ld_a_n8>,
		instruction_definition<0x40, ld_b_b>,
		instruction_definition<0x41, ld_b_c>,
		instruction_definition<0x42, ld_b_d>,
		instruction_definition<0x43, ld_b_e>,
		instruction_definition<0x44, ld_b_h>,
		instruction_definition<0x45, ld_b_l>,
		instruction_definition<0x46, ld_b_hl>,
		instruction_definition<0x47, ld_b_a>,
		instruction_definition<0x48, ld_c_b>,
		instruction_definition<0x49, ld_c_c>,
		instruction_definition<0x4A, ld_c_d>,
		instruction_definition<0x4B, ld_c_e>,
		instruction_definition<0x4C, ld_c_h>,
		instruction_definition<0x4D, ld_c_l>,
		instruction_definition<0x4E, ld_c_hl>,
		instruction_definition<0x4F, ld_c_a>,
		instruction_definition<0x50, ld_d_b>,
		instruction_definition<0x51, ld_d_c>,
		instruction_definition<0x52, ld_d_d>,
		instruction_definition<0x53, ld_d_e>,
		instruction_definition<0x54, ld_d_h>,
		instruction_definition<0x55, ld_d_l>,
		instruction_definition<0x56, ld_d_hl>,
		instruction_definition<0x57, ld_d_a>,
		instruction_definition<0x58, ld_e_b>,
		instruction_definition<0x59, ld_e_c>,
		instruction_definition<0x5A, ld_e_d>,
		instruction_definition<0x5B, ld_e_e>,
		instruction_definition<0x5C, ld_e_h>,
		instruction_definition<0x5D, ld_e_l>,
		instruction_definition<0x5E, ld_e_hl>,
		instruction_definition<0x5F, ld_e_a>,
		instruction_definition<0x60, ld_h_b>,
		instruction_definition<0x61, ld_h_c>,
		instruction_definition<0x62, ld_h_d>,
		instruction_definition<0x63, ld_h_e>,
		instruction_definition<0x64, ld_h_h>,
		instruction_definition<0x65, ld_h_l>,
		instruction_definition<0x66, ld_h_hl>,
		instruction_definition<0x67, ld_h_a>,
		instruction_definition<0x68, ld_l_b>,
		instruction_definition<0x69, ld_l_c>,
		instruction_definition<0x6A, ld_l_d>,
		instruction_definition<0x6B, ld_l_e>,
		instruction_definition<0x6C, ld_l_h>,
		instruction_definition<0x6D, ld_l_l>,
		instruction_definition<0x6E, ld_l_hl>,
		instruction_definition<0x6F, ld_l_a>,
		instruction_definition<0x70, ld_hl_b>,
		instruction_definition<0x71, ld_hl_c>,
		instruction_definition<0x72, ld_hl_d>,
		instruction_definition<0x73, ld_hl_e>,
		instruction_definition<0x74, ld_hl_h>,
		instruction_definition<0x75, ld_hl_l>,
		instruction_definition<0x77, ld_hl_a>,
		instruction_definition<0x78, ld_a_b>,
		instruction_definition<0x79, ld_a_c>,
		instruction_definition<0x7A, ld_a_d>,
		instruction_definition<0x7B, ld_a_e>,
		instruction_definition<0x7C, ld_a_h>,
		instruction_definition<0x7D, ld_a_l>,
		instruction_definition<0x7E, ld_a_hl>,
		instruction_definition<0x7F, ld_a_a>,
		instruction_definition<0x80, add_a_b>,
		instruction_definition<0x81, add_a_c>,
		instruction_definition<0x82, add_a_d>,
		instruction_definition<0x83, add_a_e>,
		instruction_definition<0x84, add_a_h>,
		instruction_definition<0x85, add_a_l>,
		instruction_definition<0x88, adc_a_b>,
		instruction_definition<0x89, adc_a_c>,
		instruction_definition<0x8A, adc_a_d>,
		instruction_definition<0x8B, adc_a_e>,
		instruction_definition<0x8C, adc_a_h>,
		instruction_definition<0x8D, adc_a_l>,
		instruction_definition<0x8F, adc_a_a>,
		instruction_definition<0xA8, xor_a_b>,
		instruction_definition<0xA9, xor_a_c>,
		instruction_definition<0xAA, xor_a_d>,
		instruction_definition<0xAB, xor_a_e>,
		instruction_definition<0xAC, xor_a_h>,
		instruction_definition<0xAD, xor_a_l>,
		instruction_definition<0xAE, xor_a_hl>,
		instruction_definition<0xAF, xor_a_a>,
		instruction_definition<0xB0, or_a_b>,
		instruction_definition<0xB1, or_a_c>,
		instruction_definition<0xB2, or_a_d>,
		instruction_definition<0xB3, or_a_e>,
		instruction_definition<0xB4, or_a_h>,
		instruction_definition<0xB5, or_a_l>,
		instruction_definition<0xB6, or_a_hl>,
		instruction_definition<0xB7, or_a_a>,
		instruction_definition<0xB8, cp_a_b>,
		instruction_definition<0xB9, cp_a_c>,
		instruction_definition<0xBA, cp_a_d>,
		instruction_definition<0xBB, cp_a_e>,
		instruction_definition<0xBC, cp_a_h>,
		instruction_definition<0xBD, cp_a_l>,
		instruction_definition<0xBF, cp_a_a>,
		instruction_definition<0xC1, pop_bc>,
		instruction_definition<0xC3, jp_n16>,
		instruction_definition<0xC4, call_nz_n16>,
		instruction_definition<0xC5, push_bc>,
		instruction_definition<0xC6, add_a_n8>,
		instruction_definition<0xC8, ret_z>,
		instruction_definition<0xC9, ret>,
		instruction_definition<0xCC, call_z_n16>,
		instruction_definition<0xCD, call_n16>,
		instruction_definition<0xCE, adc_a_n8>,
		instruction_definition<0xD0, ret_nc>,
		instruction_definition<0xD1, pop_de>,
		instruction_definition<0xD4, call_nc_n16>,
		instruction_definition<0xD5, push_de>,
		instruction_definition<0xD6, sub_a_n8>,
		instruction_definition<0xD8, ret_c>,
		instruction_definition<0xDC, call_c_n16>,
		instruction_definition<0xE0, ldh_n16_a>,
		instruction_definition<0xE1, pop_hl>,
		instruction_definition<0xE5, push_hl>,
		instruction_definition<0xE6, and_a_n8>,
		instruction_definition<0xE9, jp_hl>,
		instruction_definition<0xEA, ld_n16_a>,
		instruction_definition<0xEE, xor_a_n8>,
		instruction_definition<0xF0, ldh_a_n16>,
		instruction_definition<0xF1, pop_af>,
		instruction_definition<0xF3, di>,
		instruction_definition<0xF5, push_af>,
		instruction_definition<0xFA, ld_a_n16>,
		instruction_definition<0xFE, cp_a_n8>
	> ;

	export using default_prefixed_instruction_table_builder = instruction_table_builder<
		instruction_definition<0x18, rr_b>,
		instruction_definition<0x19, rr_c>,
		instruction_definition<0x1A, rr_d>,
		instruction_definition<0x1B, rr_e>,
		instruction_definition<0x1C, rr_h>,
		instruction_definition<0x1D, rr_l>,
		instruction_definition<0x1F, rr_a>,
		instruction_definition<0x38, srl_b>,
		instruction_definition<0x39, srl_c>,
		instruction_definition<0x3A, srl_d>,
		instruction_definition<0x3B, srl_e>,
		instruction_definition<0x3C, srl_h>,
		instruction_definition<0x3D, srl_l>,
		instruction_definition<0x3F, srl_a>
	>;
}