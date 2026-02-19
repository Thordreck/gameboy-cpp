export module opcodes:stack;

import cpu;
import std;
import utilities;
import :common;

namespace opcodes
{
	export void push_stack(cpu::cpu& cpu, const cpu::register_16::type_t value)
	{
		cpu.memory()[--cpu.sp()] = utils::most_significant_byte(value);
		cpu.memory()[--cpu.sp()] = utils::less_significant_byte(value);
	}

	export cpu::register_16::type_t pop_stack(cpu::cpu& cpu)
	{
		const auto less_significant = cpu.memory()[cpu.sp()++];
		const auto most_significant = cpu.memory()[cpu.sp()++];

		return utils::encode_little_endian(less_significant, most_significant);
	}

	template<R16RegisterProvider register_provider>
	struct pop_r16
	{
		static void execute(cpu::cpu& cpu)
		{
			register_provider::get(cpu) = pop_stack(cpu);
			cpu.pc()++;
		}
	};

	export using pop_bc = pop_r16<bc_register_provider>;
	export using pop_de = pop_r16<de_register_provider>;
	export using pop_hl = pop_r16<hl_register_provider>;

	export struct pop_af
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::uint16_t popped_result = pop_stack(cpu);
			cpu.reg().a() = popped_result >> 8;
			cpu.reg().f() = popped_result & 0xF0;

			cpu.pc()++;
		}
	};

	template<ReadOnlyR16RegisterProvider register_provider>
	struct push_r16
	{
		static void execute(cpu::cpu& cpu)
		{
			push_stack(cpu, register_provider::get(cpu));
			cpu.pc()++;
		}
	};

	export using push_bc = push_r16<bc_readonly_register_provider>;
	export using push_de = push_r16<de_readonly_register_provider>;
	export using push_hl = push_r16<hl_readonly_register_provider>;

	export struct push_af
	{
		static void execute(cpu::cpu& cpu)
		{
			push_stack(cpu, cpu.reg().af() & 0xFFF0);
			cpu.pc()++;
		}
	};

	export struct ld_sp_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.sp() = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);
			cpu.pc() += 3;
		}
	};

	export struct add_sp_e8
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::int8_t e8 = cpu.memory()[cpu.pc() + 1];
			const std::uint8_t unsigned_e8 = static_cast<std::uint8_t>(e8);
			const std::uint8_t sp_lower_byte = cpu.sp() & 0xFF;

			const bool half_overflow = utils::check_half_add_overflow(sp_lower_byte, unsigned_e8);
			const bool overflow = utils::check_add_overflow(sp_lower_byte, unsigned_e8);

			cpu.sp() = cpu.sp() + e8;

			cpu.reg().z_flag() = false;
			cpu.reg().n_flag() = false;
			cpu.reg().h_flag() = half_overflow;
			cpu.reg().c_flag() = overflow;

			cpu.pc() += 2;
		}
	};

	export struct ld_hl_sp_e8
	{
		static void execute(cpu::cpu& cpu)
		{
			const std::int8_t e8 = cpu.memory()[cpu.pc() + 1];
			const std::uint8_t unsigned_e8 = static_cast<std::uint8_t>(e8);
			const std::uint8_t sp_lower_byte = cpu.sp() & 0xFF;

			const bool half_overflow = utils::check_half_add_overflow(sp_lower_byte, unsigned_e8);
			const bool overflow = utils::check_add_overflow(sp_lower_byte, unsigned_e8);

			cpu.reg().hl() = cpu.sp() + e8;

			cpu.reg().z_flag() = false;
			cpu.reg().n_flag() = false;
			cpu.reg().h_flag() = half_overflow;
			cpu.reg().c_flag() = overflow;

			cpu.pc() += 2;
		}
	};
}