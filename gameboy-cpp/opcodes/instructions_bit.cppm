
export module opcodes:bit;

import std;
import cpu;
import utilities;
import :common;

namespace opcodes
{
	// or_a_r8
	template<ReadOnlyR8RegisterProvider reg_provider>
	struct or_a_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.reg().a() | reg_provider::get(cpu);

			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = false;

			cpu.pc()++;
		}
	};

	export using or_a_a = or_a_r8<a_readonly_register_provider>;
	export using or_a_b = or_a_r8<b_readonly_register_provider>;
	export using or_a_c = or_a_r8<c_readonly_register_provider>;
	export using or_a_d = or_a_r8<d_readonly_register_provider>;
	export using or_a_e = or_a_r8<e_readonly_register_provider>;
	export using or_a_h = or_a_r8<h_readonly_register_provider>;
	export using or_a_l = or_a_r8<l_readonly_register_provider>;

	export struct or_a_hl
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.reg().a() | cpu.memory()[cpu.reg().hl()];

			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = false;

			cpu.pc()++;
		}
	};

	export struct and_a_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.reg().a() & cpu.memory()[cpu.pc() + 1];

			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = true;
			cpu.reg().flags().c = false;

			cpu.pc() += 2;
		}
	};

	template<ReadOnlyR8RegisterProvider reg_provider>
	struct xor_a_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.reg().a() ^ reg_provider::get(cpu);

			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = false;

			cpu.pc()++;
		}
	};

	export using xor_a_a = xor_a_r8<a_readonly_register_provider>;
	export using xor_a_b = xor_a_r8<b_readonly_register_provider>;
	export using xor_a_c = xor_a_r8<c_readonly_register_provider>;
	export using xor_a_d = xor_a_r8<d_readonly_register_provider>;
	export using xor_a_e = xor_a_r8<e_readonly_register_provider>;
	export using xor_a_h = xor_a_r8<h_readonly_register_provider>;
	export using xor_a_l = xor_a_r8<l_readonly_register_provider>;

	export struct xor_a_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.reg().a() ^ cpu.memory()[cpu.pc() + 1];

			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = false;

			cpu.pc() += 2;
		}
	};

	export struct xor_a_hl
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.reg().a() ^ cpu.memory()[cpu.reg().hl()];

			cpu.reg().flags().z = cpu.reg().a() == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = false;

			cpu.pc()++;
		}
	};

	template<R8RegisterProvider reg_provider>
	struct srl_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			const bool shifted_bit = reg_provider::get(cpu) & 0x01;
			reg_provider::get(cpu) = reg_provider::get(cpu) >> 1;

			cpu.reg().flags().z = reg_provider::get(cpu) == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = shifted_bit;

			cpu.pc() += 2;
		}
	};

	export using srl_a = srl_r8<a_register_provider>;
	export using srl_b = srl_r8<b_register_provider>;
	export using srl_c = srl_r8<c_register_provider>;
	export using srl_d = srl_r8<d_register_provider>;
	export using srl_e = srl_r8<e_register_provider>;
	export using srl_h = srl_r8<h_register_provider>;
	export using srl_l = srl_r8<l_register_provider>;

	template<R8RegisterProvider reg_provider>
	struct rr_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			const bool shifted_bit = reg_provider::get(cpu) & 0x01;
			reg_provider::get(cpu) = (reg_provider::get(cpu) >> 1) | (cpu.reg().c_flag() << 7);

			cpu.reg().flags().z = reg_provider::get(cpu) == 0;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = shifted_bit;

			cpu.pc() += 2;
		}
	};

	export using rr_a = rr_r8<a_register_provider>;
	export using rr_b = rr_r8<b_register_provider>;
	export using rr_c = rr_r8<c_register_provider>;
	export using rr_d = rr_r8<d_register_provider>;
	export using rr_e = rr_r8<e_register_provider>;
	export using rr_h = rr_r8<h_register_provider>;
	export using rr_l = rr_r8<l_register_provider>;

	export struct rra
	{
		static void execute(cpu::cpu& cpu)
		{
			const bool shifted_bit = cpu.reg().a() & 0x01;
			cpu.reg().a() = (cpu.reg().a() >> 1) | (cpu.reg().c_flag() << 7);

			cpu.reg().flags().z = false;
			cpu.reg().flags().n = false;
			cpu.reg().flags().h = false;
			cpu.reg().flags().c = shifted_bit;

			cpu.pc()++;
		}
	};
}