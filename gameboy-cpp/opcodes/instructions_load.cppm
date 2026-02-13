
export module opcodes:load;

import cpu;
import std;
import :common;

namespace opcodes
{
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
}
