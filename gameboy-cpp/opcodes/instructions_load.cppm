
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
			cpu.pc()++;
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
			reg_provider::get(cpu) = cpu.memory()[++cpu.pc()];
			cpu.pc()++;
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
			reg_provider::get(cpu) = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);
			cpu.pc() += 3;
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
			cpu.memory()[cpu.reg().hl()] = reg_provider::get(cpu);
			cpu.pc()++;
		}
	};

	export using ld_hl_a = ld_hl_r8<a_readonly_register_provider>;
	export using ld_hl_b = ld_hl_r8<b_readonly_register_provider>;
	export using ld_hl_c = ld_hl_r8<c_readonly_register_provider>;
	export using ld_hl_d = ld_hl_r8<d_readonly_register_provider>;
	export using ld_hl_e = ld_hl_r8<e_readonly_register_provider>;
	export using ld_hl_h = ld_hl_r8<h_readonly_register_provider>;
	export using ld_hl_l = ld_hl_r8<l_readonly_register_provider>;

	export struct ld_hl_n8
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.memory()[cpu.reg().hl()] = cpu.memory()[cpu.pc() + 1];
			cpu.pc() += 2;
		}
	};

	// ld r8,hl
	template<R8RegisterProvider reg_provider>
	struct ld_r8_hl
	{
		static void execute(cpu::cpu& cpu)
		{
			reg_provider::get(cpu) = cpu.memory()[cpu.reg().hl()];
			cpu.pc()++;
		}
	};

	export using ld_a_hl = ld_r8_hl<a_register_provider>;
	export using ld_b_hl = ld_r8_hl<b_register_provider>;
	export using ld_c_hl = ld_r8_hl<c_register_provider>;
	export using ld_d_hl = ld_r8_hl<d_register_provider>;
	export using ld_e_hl = ld_r8_hl<e_register_provider>;
	export using ld_h_hl = ld_r8_hl<h_register_provider>;
	export using ld_l_hl = ld_r8_hl<l_register_provider>;

	export struct ld_a_hli
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.memory()[cpu.reg().hl()];
			++cpu.reg().hl();
			cpu.pc()++;
		}
	};

	export struct ld_a_hld
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.memory()[cpu.reg().hl()];
			--cpu.reg().hl();
			cpu.pc()++;
		}
	};

	export struct ld_hli_a
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.memory()[cpu.reg().hl()] = cpu.reg().a();
			++cpu.reg().hl();
			cpu.pc()++;
		}
	};

	export struct ld_hld_a
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.memory()[cpu.reg().hl()] = cpu.reg().a();
			--cpu.reg().hl();
			cpu.pc()++;
		}
	};

	// ld r16,a
	template<ReadOnlyR16RegisterProvider reg_provider>
	struct ld_r16_a
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.memory()[reg_provider::get(cpu)] = cpu.reg().a();
			cpu.pc()++;
		}
	};

	export using ld_bc_a = ld_r16_a<bc_readonly_register_provider>;
	export using ld_de_a = ld_r16_a<de_readonly_register_provider>;

	// ld n16,a
	export struct ld_n16_a
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::uint16_t address = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);

			cpu.memory()[address] = cpu.reg().a();
			cpu.pc() += 3;
		}
	};

	// ld a,n16
	export struct ld_a_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::uint16_t address = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);

			cpu.reg().a() = cpu.memory()[address];
			cpu.pc() += 3;
		}
	};

	// ld a,r16
	template <ReadOnlyR16RegisterProvider reg_provider>
	struct ld_a_r16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.reg().a() = cpu.memory()[reg_provider::get(cpu)];
			cpu.pc()++;
		}
	};

	export using ld_a_bc = ld_a_r16<bc_readonly_register_provider>;
	export using ld_a_de = ld_a_r16<de_readonly_register_provider>;

	// ldh n16,a
	export struct ldh_n16_a
	{
		static void execute(cpu::cpu& cpu)
		{
			constexpr std::uint8_t high_nibble = 0xFF;
			const std::uint16_t address = utils::encode_little_endian(cpu.memory()[cpu.pc() + 1], high_nibble);

			cpu.memory()[address] = cpu.reg().a();
			cpu.pc() += 2;
		}
	};

	// ldh n16,a
	export struct ldh_a_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			constexpr std::uint8_t high_nibble = 0xFF;
			const std::uint16_t address = utils::encode_little_endian(cpu.memory()[cpu.pc() + 1], high_nibble);

			cpu.reg().a() = cpu.memory()[address];
			cpu.pc() += 2;
		}
	};

	// ld n16,sp
	export struct ld_n16_sp
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::uint16_t address = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);
			utils::write_two_byte_little_endian(cpu.sp().value(), cpu.memory(), address);

			cpu.pc() += 3;
		}
	};

	// ld sp,hl
	export struct ld_sp_hl
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.sp() = cpu.reg().hl();
			cpu.pc()++;
		}
	};
}
