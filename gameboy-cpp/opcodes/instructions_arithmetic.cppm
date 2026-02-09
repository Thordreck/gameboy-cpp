
export module opcodes:arithmetic;

export import std;
export import cpu;
export import utilities;

namespace opcodes 
{
	export template<auto RegFn>
	struct add_a_r8
	{
		static constexpr auto reg_fn = RegFn;

		static void execute(cpu::cpu& cpu)
		{
			const cpu::register_8 r8 = (cpu.registers.*reg_fn)();
			const bool carry = utils::check_add_overflow<cpu::register_8::type_t>(cpu.registers.a(), r8);
			const bool half_carry = utils::check_half_add_overflow(cpu.registers.a(), r8);

			cpu.registers.a() = cpu.registers.a() + r8;
			cpu.registers.flags().z = cpu.registers.a() == 0;
			cpu.registers.flags().n = false;
			cpu.registers.flags().h = half_carry;
			cpu.registers.flags().c = carry;
			++cpu.pc;
		}
	};

	export using add_a_b = add_a_r8<&cpu::registers::b>;
	export using add_a_c = add_a_r8<&cpu::registers::c>;
	export using add_a_d = add_a_r8<&cpu::registers::d>;
	export using add_a_e = add_a_r8<&cpu::registers::e>;
	export using add_a_h = add_a_r8<&cpu::registers::h>;
	export using add_a_l = add_a_r8<&cpu::registers::l>;
}
