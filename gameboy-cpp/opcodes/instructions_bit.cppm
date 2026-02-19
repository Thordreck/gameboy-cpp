
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

	template<std::uint8_t index, ReadOnlyR8RegisterProvider reg_provider>
	requires BitIndex<index>
	struct bit_u3_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().z_flag() = !utils::is_bit_set<index>(reg_provider::get(cpu).value());
			cpu.reg().n_flag() = false;
			cpu.reg().h_flag() = true;

			cpu.pc() += 2;
		}
	};

	export using bit_0_a = bit_u3_r8<0, a_readonly_register_provider>;
	export using bit_1_a = bit_u3_r8<1, a_readonly_register_provider>;
	export using bit_2_a = bit_u3_r8<2, a_readonly_register_provider>;
	export using bit_3_a = bit_u3_r8<3, a_readonly_register_provider>;
	export using bit_4_a = bit_u3_r8<4, a_readonly_register_provider>;
	export using bit_5_a = bit_u3_r8<5, a_readonly_register_provider>;
	export using bit_6_a = bit_u3_r8<6, a_readonly_register_provider>;
	export using bit_7_a = bit_u3_r8<7, a_readonly_register_provider>;

	export using bit_0_b = bit_u3_r8<0, b_readonly_register_provider>;
	export using bit_1_b = bit_u3_r8<1, b_readonly_register_provider>;
	export using bit_2_b = bit_u3_r8<2, b_readonly_register_provider>;
	export using bit_3_b = bit_u3_r8<3, b_readonly_register_provider>;
	export using bit_4_b = bit_u3_r8<4, b_readonly_register_provider>;
	export using bit_5_b = bit_u3_r8<5, b_readonly_register_provider>;
	export using bit_6_b = bit_u3_r8<6, b_readonly_register_provider>;
	export using bit_7_b = bit_u3_r8<7, b_readonly_register_provider>;

	export using bit_0_c = bit_u3_r8<0, c_readonly_register_provider>;
	export using bit_1_c = bit_u3_r8<1, c_readonly_register_provider>;
	export using bit_2_c = bit_u3_r8<2, c_readonly_register_provider>;
	export using bit_3_c = bit_u3_r8<3, c_readonly_register_provider>;
	export using bit_4_c = bit_u3_r8<4, c_readonly_register_provider>;
	export using bit_5_c = bit_u3_r8<5, c_readonly_register_provider>;
	export using bit_6_c = bit_u3_r8<6, c_readonly_register_provider>;
	export using bit_7_c = bit_u3_r8<7, c_readonly_register_provider>;

	export using bit_0_d = bit_u3_r8<0, d_readonly_register_provider>;
	export using bit_1_d = bit_u3_r8<1, d_readonly_register_provider>;
	export using bit_2_d = bit_u3_r8<2, d_readonly_register_provider>;
	export using bit_3_d = bit_u3_r8<3, d_readonly_register_provider>;
	export using bit_4_d = bit_u3_r8<4, d_readonly_register_provider>;
	export using bit_5_d = bit_u3_r8<5, d_readonly_register_provider>;
	export using bit_6_d = bit_u3_r8<6, d_readonly_register_provider>;
	export using bit_7_d = bit_u3_r8<7, d_readonly_register_provider>;

	export using bit_0_e = bit_u3_r8<0, e_readonly_register_provider>;
	export using bit_1_e = bit_u3_r8<1, e_readonly_register_provider>;
	export using bit_2_e = bit_u3_r8<2, e_readonly_register_provider>;
	export using bit_3_e = bit_u3_r8<3, e_readonly_register_provider>;
	export using bit_4_e = bit_u3_r8<4, e_readonly_register_provider>;
	export using bit_5_e = bit_u3_r8<5, e_readonly_register_provider>;
	export using bit_6_e = bit_u3_r8<6, e_readonly_register_provider>;
	export using bit_7_e = bit_u3_r8<7, e_readonly_register_provider>;

	export using bit_0_h = bit_u3_r8<0, h_readonly_register_provider>;
	export using bit_1_h = bit_u3_r8<1, h_readonly_register_provider>;
	export using bit_2_h = bit_u3_r8<2, h_readonly_register_provider>;
	export using bit_3_h = bit_u3_r8<3, h_readonly_register_provider>;
	export using bit_4_h = bit_u3_r8<4, h_readonly_register_provider>;
	export using bit_5_h = bit_u3_r8<5, h_readonly_register_provider>;
	export using bit_6_h = bit_u3_r8<6, h_readonly_register_provider>;
	export using bit_7_h = bit_u3_r8<7, h_readonly_register_provider>;

	export using bit_0_l = bit_u3_r8<0, l_readonly_register_provider>;
	export using bit_1_l = bit_u3_r8<1, l_readonly_register_provider>;
	export using bit_2_l = bit_u3_r8<2, l_readonly_register_provider>;
	export using bit_3_l = bit_u3_r8<3, l_readonly_register_provider>;
	export using bit_4_l = bit_u3_r8<4, l_readonly_register_provider>;
	export using bit_5_l = bit_u3_r8<5, l_readonly_register_provider>;
	export using bit_6_l = bit_u3_r8<6, l_readonly_register_provider>;
	export using bit_7_l = bit_u3_r8<7, l_readonly_register_provider>;

	template<std::uint8_t index, R8RegisterProvider reg_provider>
	requires BitIndex<index>
	struct res_u3_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			reg_provider::get(cpu) = reg_provider::get(cpu) & ~(0x1 << index);
			cpu.pc() += 2;
		}
	};

	export using res_0_a = res_u3_r8<0, a_register_provider>;
	export using res_1_a = res_u3_r8<1, a_register_provider>;
	export using res_2_a = res_u3_r8<2, a_register_provider>;
	export using res_3_a = res_u3_r8<3, a_register_provider>;
	export using res_4_a = res_u3_r8<4, a_register_provider>;
	export using res_5_a = res_u3_r8<5, a_register_provider>;
	export using res_6_a = res_u3_r8<6, a_register_provider>;
	export using res_7_a = res_u3_r8<7, a_register_provider>;

	export using res_0_b = res_u3_r8<0, b_register_provider>;
	export using res_1_b = res_u3_r8<1, b_register_provider>;
	export using res_2_b = res_u3_r8<2, b_register_provider>;
	export using res_3_b = res_u3_r8<3, b_register_provider>;
	export using res_4_b = res_u3_r8<4, b_register_provider>;
	export using res_5_b = res_u3_r8<5, b_register_provider>;
	export using res_6_b = res_u3_r8<6, b_register_provider>;
	export using res_7_b = res_u3_r8<7, b_register_provider>;

	export using res_0_c = res_u3_r8<0, c_register_provider>;
	export using res_1_c = res_u3_r8<1, c_register_provider>;
	export using res_2_c = res_u3_r8<2, c_register_provider>;
	export using res_3_c = res_u3_r8<3, c_register_provider>;
	export using res_4_c = res_u3_r8<4, c_register_provider>;
	export using res_5_c = res_u3_r8<5, c_register_provider>;
	export using res_6_c = res_u3_r8<6, c_register_provider>;
	export using res_7_c = res_u3_r8<7, c_register_provider>;

	export using res_0_d = res_u3_r8<0, d_register_provider>;
	export using res_1_d = res_u3_r8<1, d_register_provider>;
	export using res_2_d = res_u3_r8<2, d_register_provider>;
	export using res_3_d = res_u3_r8<3, d_register_provider>;
	export using res_4_d = res_u3_r8<4, d_register_provider>;
	export using res_5_d = res_u3_r8<5, d_register_provider>;
	export using res_6_d = res_u3_r8<6, d_register_provider>;
	export using res_7_d = res_u3_r8<7, d_register_provider>;

	export using res_0_e = res_u3_r8<0, e_register_provider>;
	export using res_1_e = res_u3_r8<1, e_register_provider>;
	export using res_2_e = res_u3_r8<2, e_register_provider>;
	export using res_3_e = res_u3_r8<3, e_register_provider>;
	export using res_4_e = res_u3_r8<4, e_register_provider>;
	export using res_5_e = res_u3_r8<5, e_register_provider>;
	export using res_6_e = res_u3_r8<6, e_register_provider>;
	export using res_7_e = res_u3_r8<7, e_register_provider>;

	export using res_0_h = res_u3_r8<0, h_register_provider>;
	export using res_1_h = res_u3_r8<1, h_register_provider>;
	export using res_2_h = res_u3_r8<2, h_register_provider>;
	export using res_3_h = res_u3_r8<3, h_register_provider>;
	export using res_4_h = res_u3_r8<4, h_register_provider>;
	export using res_5_h = res_u3_r8<5, h_register_provider>;
	export using res_6_h = res_u3_r8<6, h_register_provider>;
	export using res_7_h = res_u3_r8<7, h_register_provider>;

	export using res_0_l = res_u3_r8<0, l_register_provider>;
	export using res_1_l = res_u3_r8<1, l_register_provider>;
	export using res_2_l = res_u3_r8<2, l_register_provider>;
	export using res_3_l = res_u3_r8<3, l_register_provider>;
	export using res_4_l = res_u3_r8<4, l_register_provider>;
	export using res_5_l = res_u3_r8<5, l_register_provider>;
	export using res_6_l = res_u3_r8<6, l_register_provider>;
	export using res_7_l = res_u3_r8<7, l_register_provider>;

	template<std::uint8_t index, R8RegisterProvider reg_provider>
	requires BitIndex<index>
	struct set_u3_r8
	{
		static void execute(cpu::cpu& cpu)
		{
			reg_provider::get(cpu) = reg_provider::get(cpu) | (0x1 << index);
			cpu.pc() += 2;
		}
	};

	export using set_0_a = set_u3_r8<0, a_register_provider>;
	export using set_1_a = set_u3_r8<1, a_register_provider>;
	export using set_2_a = set_u3_r8<2, a_register_provider>;
	export using set_3_a = set_u3_r8<3, a_register_provider>;
	export using set_4_a = set_u3_r8<4, a_register_provider>;
	export using set_5_a = set_u3_r8<5, a_register_provider>;
	export using set_6_a = set_u3_r8<6, a_register_provider>;
	export using set_7_a = set_u3_r8<7, a_register_provider>;

	export using set_0_b = set_u3_r8<0, b_register_provider>;
	export using set_1_b = set_u3_r8<1, b_register_provider>;
	export using set_2_b = set_u3_r8<2, b_register_provider>;
	export using set_3_b = set_u3_r8<3, b_register_provider>;
	export using set_4_b = set_u3_r8<4, b_register_provider>;
	export using set_5_b = set_u3_r8<5, b_register_provider>;
	export using set_6_b = set_u3_r8<6, b_register_provider>;
	export using set_7_b = set_u3_r8<7, b_register_provider>;

	export using set_0_c = set_u3_r8<0, c_register_provider>;
	export using set_1_c = set_u3_r8<1, c_register_provider>;
	export using set_2_c = set_u3_r8<2, c_register_provider>;
	export using set_3_c = set_u3_r8<3, c_register_provider>;
	export using set_4_c = set_u3_r8<4, c_register_provider>;
	export using set_5_c = set_u3_r8<5, c_register_provider>;
	export using set_6_c = set_u3_r8<6, c_register_provider>;
	export using set_7_c = set_u3_r8<7, c_register_provider>;

	export using set_0_d = set_u3_r8<0, d_register_provider>;
	export using set_1_d = set_u3_r8<1, d_register_provider>;
	export using set_2_d = set_u3_r8<2, d_register_provider>;
	export using set_3_d = set_u3_r8<3, d_register_provider>;
	export using set_4_d = set_u3_r8<4, d_register_provider>;
	export using set_5_d = set_u3_r8<5, d_register_provider>;
	export using set_6_d = set_u3_r8<6, d_register_provider>;
	export using set_7_d = set_u3_r8<7, d_register_provider>;

	export using set_0_e = set_u3_r8<0, e_register_provider>;
	export using set_1_e = set_u3_r8<1, e_register_provider>;
	export using set_2_e = set_u3_r8<2, e_register_provider>;
	export using set_3_e = set_u3_r8<3, e_register_provider>;
	export using set_4_e = set_u3_r8<4, e_register_provider>;
	export using set_5_e = set_u3_r8<5, e_register_provider>;
	export using set_6_e = set_u3_r8<6, e_register_provider>;
	export using set_7_e = set_u3_r8<7, e_register_provider>;

	export using set_0_h = set_u3_r8<0, h_register_provider>;
	export using set_1_h = set_u3_r8<1, h_register_provider>;
	export using set_2_h = set_u3_r8<2, h_register_provider>;
	export using set_3_h = set_u3_r8<3, h_register_provider>;
	export using set_4_h = set_u3_r8<4, h_register_provider>;
	export using set_5_h = set_u3_r8<5, h_register_provider>;
	export using set_6_h = set_u3_r8<6, h_register_provider>;
	export using set_7_h = set_u3_r8<7, h_register_provider>;

	export using set_0_l = set_u3_r8<0, l_register_provider>;
	export using set_1_l = set_u3_r8<1, l_register_provider>;
	export using set_2_l = set_u3_r8<2, l_register_provider>;
	export using set_3_l = set_u3_r8<3, l_register_provider>;
	export using set_4_l = set_u3_r8<4, l_register_provider>;
	export using set_5_l = set_u3_r8<5, l_register_provider>;
	export using set_6_l = set_u3_r8<6, l_register_provider>;
	export using set_7_l = set_u3_r8<7, l_register_provider>;
}