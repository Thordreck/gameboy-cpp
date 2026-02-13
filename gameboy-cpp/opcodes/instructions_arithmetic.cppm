
export module opcodes:arithmetic;

import std;
import cpu;
import utilities;
import :common;

namespace opcodes 
{
	export template<ReadOnlyR8RegisterProvider reg_provider>
	struct add_a_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			const cpu::readonly_register_8 r8 = reg_provider::get(cpu);
			const bool carry = utils::check_add_overflow<cpu::register_8::type_t>(cpu.registers.a(), r8);
			const bool half_carry = utils::check_half_add_overflow(cpu.registers.a(), r8);

			cpu.registers.a() = cpu.registers.a() + r8;
			cpu.registers.flags().z = cpu.registers.a() == 0;
			cpu.registers.flags().n = false;
			cpu.registers.flags().h = half_carry;
			cpu.registers.flags().c = carry;
			cpu.pc++;
		}
	};

	export using add_a_b = add_a_r8<b_readonly_register_provider> ;
	export using add_a_c = add_a_r8<c_readonly_register_provider>;
	export using add_a_d = add_a_r8<d_readonly_register_provider>;
	export using add_a_e = add_a_r8<e_readonly_register_provider>;
	export using add_a_h = add_a_r8<h_readonly_register_provider>;
	export using add_a_l = add_a_r8<l_readonly_register_provider>;
}
