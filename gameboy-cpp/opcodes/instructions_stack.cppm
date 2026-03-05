export module opcodes:stack;

import cpu;
import std;
import utilities;
import :common;

namespace opcodes
{
	using namespace cpu::literals;

	template<R16RegisterProvider register_provider>
	struct pop_r16
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 3_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory().read(cpu.sp()++);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint8_t low_byte = cpu.cache().r8;
				const std::uint8_t high_byte = cpu.memory().read(cpu.sp()++);

				register_provider::get(cpu) = utils::encode_little_endian(low_byte, high_byte);
			}
		}
	};

	export using pop_bc = pop_r16<bc_register_provider>;
	export using pop_de = pop_r16<de_register_provider>;
	export using pop_hl = pop_r16<hl_register_provider>;

	export struct pop_af
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 3_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory().read(cpu.sp()++);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint8_t low_byte = cpu.cache().r8;
				const std::uint8_t high_byte = cpu.memory().read(cpu.sp()++);
				const std::uint16_t popped_result = utils::encode_little_endian(low_byte, high_byte);

				cpu.reg().a() = popped_result >> 8;
				cpu.reg().f() = popped_result & 0xF0;
			}
		}
	};

	template<ReadOnlyR16RegisterProvider register_provider>
	struct push_r16
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r16 = register_provider::get(cpu);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				cpu.memory().write(--cpu.sp(), utils::most_significant_byte(cpu.cache().r16));
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				cpu.memory().write(--cpu.sp(), utils::less_significant_byte(cpu.cache().r16));
			}
		}
	};

	export using push_bc = push_r16<bc_readonly_register_provider>;
	export using push_de = push_r16<de_readonly_register_provider>;
	export using push_hl = push_r16<hl_readonly_register_provider>;

	export struct push_af
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r16 = cpu.reg().af() & 0xFFF0;
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint16_t value = cpu.cache().r16;
				cpu.memory().write(--cpu.sp(), utils::most_significant_byte(value));
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				const std::uint16_t value = cpu.cache().r16;
				cpu.memory().write(--cpu.sp(), utils::less_significant_byte(value));
			}
		}
	};

	export struct ld_sp_n16
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 3_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory().read(cpu.pc()++);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint8_t low_byte = cpu.cache().r8;
				const std::uint8_t high_byte = cpu.memory().read(cpu.pc()++);

				cpu.sp() = utils::encode_little_endian(low_byte, high_byte);
			}
		}
	};

	export struct add_sp_e8
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory().read(cpu.pc()++);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint8_t unsigned_e8 = cpu.cache().r8;
				const std::uint8_t sp_lower_byte = cpu.sp() & 0xFF;

				const bool half_overflow = utils::check_half_add_overflow(sp_lower_byte, unsigned_e8);
				const bool overflow = utils::check_add_overflow(sp_lower_byte, unsigned_e8);

				cpu.reg().z_flag() = false;
				cpu.reg().n_flag() = false;
				cpu.reg().h_flag() = half_overflow;
				cpu.reg().c_flag() = overflow;
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				const std::int8_t e8 = static_cast<int8_t>(cpu.cache().r8);
				cpu.sp() = cpu.sp() + e8;
			}
		}
	};

	export struct ld_hl_sp_e8
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 3_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory().read(cpu.pc()++);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint8_t unsigned_e8 = cpu.cache().r8;
				const std::int8_t e8 = static_cast<std::int8_t>(unsigned_e8);
				const std::uint8_t sp_lower_byte = cpu.sp() & 0xFF;

				const bool half_overflow = utils::check_half_add_overflow(sp_lower_byte, unsigned_e8);
				const bool overflow = utils::check_add_overflow(sp_lower_byte, unsigned_e8);

				cpu.reg().hl() = cpu.sp() + e8;

				cpu.reg().z_flag() = false;
				cpu.reg().n_flag() = false;
				cpu.reg().h_flag() = half_overflow;
				cpu.reg().c_flag() = overflow;
			}
		}
	};
}