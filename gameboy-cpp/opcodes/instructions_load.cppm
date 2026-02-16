
export module opcodes:load;

import cpu;
import std;
import utilities;
import :common;

namespace opcodes
{
	// ld_r8_r8
	template<R8RegisterProvider lhs_provider, ReadOnlyR8RegisterProvider rhs_provider>
	struct ld_r8_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			lhs_provider::get(cpu) = rhs_provider::get(cpu);
			cpu.pc++;
		}
	};

	// ld_a_r8
	export using ld_a_a = ld_r8_r8<a_register_provider, a_readonly_register_provider>;
	export using ld_a_b = ld_r8_r8<a_register_provider, b_readonly_register_provider>;
	export using ld_a_c = ld_r8_r8<a_register_provider, c_readonly_register_provider>;
	export using ld_a_d = ld_r8_r8<a_register_provider, d_readonly_register_provider>;
	export using ld_a_e = ld_r8_r8<a_register_provider, e_readonly_register_provider>;
	export using ld_a_h = ld_r8_r8<a_register_provider, h_readonly_register_provider>;
	export using ld_a_l = ld_r8_r8<a_register_provider, l_readonly_register_provider>;

	// ld_b_r8
	export using ld_b_a = ld_r8_r8<b_register_provider, a_readonly_register_provider>;
	export using ld_b_b = ld_r8_r8<b_register_provider, b_readonly_register_provider>;
	export using ld_b_c = ld_r8_r8<b_register_provider, c_readonly_register_provider>;
	export using ld_b_d = ld_r8_r8<b_register_provider, d_readonly_register_provider>;
	export using ld_b_e = ld_r8_r8<b_register_provider, e_readonly_register_provider>;
	export using ld_b_h = ld_r8_r8<b_register_provider, h_readonly_register_provider>;
	export using ld_b_l = ld_r8_r8<b_register_provider, l_readonly_register_provider>;

	// ld_c_r8
	export using ld_c_a = ld_r8_r8<c_register_provider, a_readonly_register_provider>;
	export using ld_c_b = ld_r8_r8<c_register_provider, b_readonly_register_provider>;
	export using ld_c_c = ld_r8_r8<c_register_provider, c_readonly_register_provider>;
	export using ld_c_d = ld_r8_r8<c_register_provider, d_readonly_register_provider>;
	export using ld_c_e = ld_r8_r8<c_register_provider, e_readonly_register_provider>;
	export using ld_c_h = ld_r8_r8<c_register_provider, h_readonly_register_provider>;
	export using ld_c_l = ld_r8_r8<c_register_provider, l_readonly_register_provider>;

	// ld_d_r8
	export using ld_d_a = ld_r8_r8<d_register_provider, a_readonly_register_provider>;
	export using ld_d_b = ld_r8_r8<d_register_provider, b_readonly_register_provider>;
	export using ld_d_c = ld_r8_r8<d_register_provider, c_readonly_register_provider>;
	export using ld_d_d = ld_r8_r8<d_register_provider, d_readonly_register_provider>;
	export using ld_d_e = ld_r8_r8<d_register_provider, e_readonly_register_provider>;
	export using ld_d_h = ld_r8_r8<d_register_provider, h_readonly_register_provider>;
	export using ld_d_l = ld_r8_r8<d_register_provider, l_readonly_register_provider>;

	// ld_e_r8
	export using ld_e_a = ld_r8_r8<e_register_provider, a_readonly_register_provider>;
	export using ld_e_b = ld_r8_r8<e_register_provider, b_readonly_register_provider>;
	export using ld_e_c = ld_r8_r8<e_register_provider, c_readonly_register_provider>;
	export using ld_e_d = ld_r8_r8<e_register_provider, d_readonly_register_provider>;
	export using ld_e_e = ld_r8_r8<e_register_provider, e_readonly_register_provider>;
	export using ld_e_h = ld_r8_r8<e_register_provider, h_readonly_register_provider>;
	export using ld_e_l = ld_r8_r8<e_register_provider, l_readonly_register_provider>;

	// ld_h_r8
	export using ld_h_a = ld_r8_r8<h_register_provider, a_readonly_register_provider>;
	export using ld_h_b = ld_r8_r8<h_register_provider, b_readonly_register_provider>;
	export using ld_h_c = ld_r8_r8<h_register_provider, c_readonly_register_provider>;
	export using ld_h_d = ld_r8_r8<h_register_provider, d_readonly_register_provider>;
	export using ld_h_e = ld_r8_r8<h_register_provider, e_readonly_register_provider>;
	export using ld_h_h = ld_r8_r8<h_register_provider, h_readonly_register_provider>;
	export using ld_h_l = ld_r8_r8<h_register_provider, l_readonly_register_provider>;

	// ld_l_r8
	export using ld_l_a = ld_r8_r8<l_register_provider, a_readonly_register_provider>;
	export using ld_l_b = ld_r8_r8<l_register_provider, b_readonly_register_provider>;
	export using ld_l_c = ld_r8_r8<l_register_provider, c_readonly_register_provider>;
	export using ld_l_d = ld_r8_r8<l_register_provider, d_readonly_register_provider>;
	export using ld_l_e = ld_r8_r8<l_register_provider, e_readonly_register_provider>;
	export using ld_l_h = ld_r8_r8<l_register_provider, h_readonly_register_provider>;
	export using ld_l_l = ld_r8_r8<l_register_provider, l_readonly_register_provider>;

	// ld_r8_n8
	template<R8RegisterProvider reg_provider>
	struct ld_r8_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			reg_provider::get(cpu) = cpu.memory[++cpu.pc];
			cpu.pc++;
		}
	};

	export using ld_a_n8 = ld_r8_n8<a_register_provider>;
	export using ld_b_n8 = ld_r8_n8<b_register_provider>;
	export using ld_c_n8 = ld_r8_n8<c_register_provider>;
	export using ld_d_n8 = ld_r8_n8<d_register_provider>;
	export using ld_e_n8 = ld_r8_n8<e_register_provider>;
	export using ld_h_n8 = ld_r8_n8<h_register_provider>;
	export using ld_l_n8 = ld_r8_n8<l_register_provider>;

	// ld r16,n16
	template<R16RegisterProvider reg_provider>
	struct ld_r16_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			reg_provider::get(cpu) = utils::read_two_byte_little_endian(cpu.memory, cpu.pc + 1);
			cpu.pc += 3;
		}
	};

	export using ld_bc_n16 = ld_r16_n16<bc_register_provider>;
	export using ld_de_n16 = ld_r16_n16<de_register_provider>;
	export using ld_hl_n16 = ld_r16_n16<hl_register_provider>;

	// ld hl,r8
	template<ReadOnlyR8RegisterProvider reg_provider>
	struct ld_hl_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.memory[cpu.registers.hl()] = reg_provider::get(cpu);
			cpu.pc += 2;
		}
	};

	export using ld_hl_a = ld_hl_r8<a_readonly_register_provider>;
	export using ld_hl_b = ld_hl_r8<b_readonly_register_provider>;
	export using ld_hl_c = ld_hl_r8<c_readonly_register_provider>;
	export using ld_hl_d = ld_hl_r8<d_readonly_register_provider>;
	export using ld_hl_e = ld_hl_r8<e_readonly_register_provider>;
	export using ld_hl_h = ld_hl_r8<h_readonly_register_provider>;
	export using ld_hl_l = ld_hl_r8<l_readonly_register_provider>;
}
