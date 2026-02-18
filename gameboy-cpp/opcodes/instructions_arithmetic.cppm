
export module opcodes:arithmetic;

import std;
import cpu;
import utilities;
import :common;

namespace opcodes 
{
	export struct add_a_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::uint8_t n8 = cpu.memory()[cpu.pc() + 1];
			const bool carry = utils::check_add_overflow(cpu.reg().a().value(), n8);
			const bool half_carry = utils::check_half_add_overflow(cpu.reg().a().value(), n8);

			cpu.reg().a() = cpu.reg().a() + n8;
			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = half_carry;
			cpu.reg().flags().c = carry;
			cpu.pc() += 2;
		}
	};

	// add_a_r8
	template<ReadOnlyR8RegisterProvider reg_provider>
	struct add_a_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			const cpu::readonly_register_8 r8 = reg_provider::get(cpu);
			const bool carry = utils::check_add_overflow(cpu.reg().a().value(), r8.value());
			const bool half_carry = utils::check_half_add_overflow(cpu.reg().a().value(), r8.value());

			cpu.reg().a() = cpu.reg().a() + r8;
			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = half_carry;
			cpu.reg().flags().c = carry;
			cpu.pc()++;
		}
	};

	export using add_a_b = add_a_r8<b_readonly_register_provider>;
	export using add_a_c = add_a_r8<c_readonly_register_provider>;
	export using add_a_d = add_a_r8<d_readonly_register_provider>;
	export using add_a_e = add_a_r8<e_readonly_register_provider>;
	export using add_a_h = add_a_r8<h_readonly_register_provider>;
	export using add_a_l = add_a_r8<l_readonly_register_provider>;

	// inc_r8
	template<R8RegisterProvider reg_provider>
	struct inc_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			using namespace literals;

			cpu::register_8 r8 = reg_provider::get(cpu);
			const bool half_carry = utils::check_half_add_overflow(r8.value(), 1_u8);

			++r8;
			cpu.reg().flags().z = r8 == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = half_carry;
			cpu.pc()++;
		}
	};

	export using inc_a = inc_r8<a_register_provider>;
	export using inc_b = inc_r8<b_register_provider>;
	export using inc_c = inc_r8<c_register_provider>;
	export using inc_d = inc_r8<d_register_provider>;
	export using inc_e = inc_r8<e_register_provider>;
	export using inc_h = inc_r8<h_register_provider>;
	export using inc_l = inc_r8<l_register_provider>;

	template<R8RegisterProvider reg_provider>
	struct dec_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			using namespace literals;

			cpu::register_8 r8 = reg_provider::get(cpu);
			const bool half_underflow = utils::check_half_substract_underflow(r8.value(), 1_u8);

			--r8;
			cpu.reg().flags().z = r8 == 0;
			cpu.reg().flags().n = true;
			cpu.reg().flags().h = half_underflow;
			cpu.pc()++;
		}
	};

	export using dec_a = dec_r8<a_register_provider>;
	export using dec_b = dec_r8<b_register_provider>;
	export using dec_c = dec_r8<c_register_provider>;
	export using dec_d = dec_r8<d_register_provider>;
	export using dec_e = dec_r8<e_register_provider>;
	export using dec_h = dec_r8<h_register_provider>;
	export using dec_l = dec_r8<l_register_provider>;

	export struct dec_hl
	{
		static void execute(cpu::cpu& cpu)
		{
			using namespace literals;

			cpu::memory_bus::type_t& n8 = cpu.memory()[cpu.reg().hl()];
			const bool half_underflow = utils::check_half_substract_underflow(n8, 1_u8);

			--n8;
			cpu.reg().flags().z = n8 == 0;
			cpu.reg().flags().n = true;
			cpu.reg().flags().h = half_underflow;
			cpu.pc()++;
		}
	};

	// inc_r16
	template<R16RegisterProvider reg_provider>
	struct inc_r16
	{
		static void execute(cpu::cpu& cpu)
		{
			++reg_provider::get(cpu);
			cpu.pc()++;
		}
	};

	export using inc_bc = inc_r16<bc_register_provider>;
	export using inc_de = inc_r16<de_register_provider>;
	export using inc_hl = inc_r16<hl_register_provider>;

	template<ReadOnlyR8RegisterProvider reg_provider>
	struct cp_a_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			const auto a = cpu.reg().a();
			const auto r8 = reg_provider::get(cpu);

			const bool underflow = utils::check_substract_underflow(a.value(), r8.value());
			const bool half_underflow = utils::check_half_substract_underflow(a.value(), r8.value());

			cpu.reg().flags().z = (a - r8) == 0;
			cpu.reg().flags().n = true;
			cpu.reg().flags().c = underflow;
			cpu.reg().flags().h = half_underflow;

			cpu.pc()++;
		}
	};

	export using cp_a_a = cp_a_r8<a_readonly_register_provider>;
	export using cp_a_b = cp_a_r8<b_readonly_register_provider>;
	export using cp_a_c = cp_a_r8<c_readonly_register_provider>;
	export using cp_a_d = cp_a_r8<d_readonly_register_provider>;
	export using cp_a_e = cp_a_r8<e_readonly_register_provider>;
	export using cp_a_h = cp_a_r8<h_readonly_register_provider>;
	export using cp_a_l = cp_a_r8<l_readonly_register_provider>;

	export struct cp_a_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			const auto a = cpu.reg().a();
			const auto n8 = cpu.memory()[cpu.pc() + 1];

			const bool underflow = utils::check_substract_underflow(a.value(), n8);
			const bool half_underflow = utils::check_half_substract_underflow(a.value(), n8);

			cpu.reg().flags().z = (a - n8) == 0;
			cpu.reg().flags().n = true;
			cpu.reg().flags().c = underflow;
			cpu.reg().flags().h = half_underflow;

			cpu.pc() += 2;
		}
	};

	export struct sub_a_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::uint8_t n8 = cpu.memory()[cpu.pc() + 1];
			const bool underflow = utils::check_substract_underflow(cpu.reg().a().value(), n8);
			const bool half_underflow = utils::check_half_substract_underflow(cpu.reg().a().value(), n8);

			cpu.reg().a() = cpu.reg().a() - n8;
			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = true;
			cpu.reg().flags().h = half_underflow;
			cpu.reg().flags().c = underflow;

			cpu.pc() += 2;
		}
	};

	template<ReadOnlyR8RegisterProvider reg_provider>
	struct adc_a_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu::register_8 a = cpu.reg().a();

			const cpu::register_8::type_t rhs = reg_provider::get(cpu) + cpu.reg().c_flag();
			const bool overflow = utils::check_add_overflow(a.value(), rhs);
			const bool half_overflow = utils::check_half_add_overflow(a.value(), rhs);

			a = a + rhs;
			cpu.reg().flags().z = a == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().c = overflow;
			cpu.reg().flags().h = half_overflow;

			cpu.pc()++;
		}
	};

	export using adc_a_a = adc_a_r8<a_readonly_register_provider>;
	export using adc_a_b = adc_a_r8<b_readonly_register_provider>;
	export using adc_a_c = adc_a_r8<c_readonly_register_provider>;
	export using adc_a_d = adc_a_r8<d_readonly_register_provider>;
	export using adc_a_e = adc_a_r8<e_readonly_register_provider>;
	export using adc_a_h = adc_a_r8<h_readonly_register_provider>;
	export using adc_a_l = adc_a_r8<l_readonly_register_provider>;

	export struct adc_a_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu::register_8 a = cpu.reg().a();
			const cpu::memory_bus::type_t n8 = cpu.memory()[cpu.pc() + 1];
			const cpu::memory_bus::type_t rhs = n8 + cpu.reg().c_flag();

			const bool overflow = utils::check_add_overflow(a.value(), rhs);
			const bool half_overflow = utils::check_half_add_overflow(a.value(), rhs);

			a = a + rhs;
			cpu.reg().flags().z = a == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().c = overflow;
			cpu.reg().flags().h = half_overflow;

			cpu.pc() += 2;
		}
	};

	template<ReadOnlyR16RegisterProvider reg_provider>
	struct add_hl_r16
	{
		static void execute(cpu::cpu& cpu)
		{
			const bool overflow = utils::check_add_overflow(cpu.reg().hl().value(), reg_provider::get(cpu).value());
			const bool half_overflow = utils::check_half_add_overflow(cpu.reg().hl().value(), reg_provider::get(cpu).value());

			cpu.reg().hl() = cpu.reg().hl() + reg_provider::get(cpu);
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = half_overflow;
			cpu.reg().flags().c = overflow;

			cpu.pc()++;
		}
	};

	export using add_hl_bc = add_hl_r16<bc_readonly_register_provider>;
	export using add_hl_de = add_hl_r16<de_readonly_register_provider>;
	export using add_hl_hl = add_hl_r16<hl_readonly_register_provider>;

	export struct add_hl_sp
	{
		static void execute(cpu::cpu& cpu)
		{
			const bool overflow = utils::check_add_overflow(cpu.reg().hl().value(), cpu.sp().value());
			const bool half_overflow = utils::check_half_add_overflow(cpu.reg().hl().value(), cpu.sp().value());

			cpu.reg().hl() = cpu.reg().hl() + cpu.sp();
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = half_overflow;
			cpu.reg().flags().c = overflow;

			cpu.pc()++;
		}
	};
}
