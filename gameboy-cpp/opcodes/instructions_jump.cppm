
export module opcodes:jump;

import :common;
import cpu;
import utilities;
import std;

namespace opcodes
{
	using namespace cpu::literals;

	export struct jp_n16
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.pc() = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);
		}
	};

	template <CPUStateCondition condition>
	struct jp_cc_n16
	{
		static constexpr auto num_cycles(const cpu::cpu& cpu) 
		{ 
			return condition::evaluate(cpu) ? 4_m_cycle : 3_m_cycle; 
		}

		static void execute(cpu::cpu& cpu)
		{
			if (condition::evaluate(cpu))
			{
				jp_n16::execute(cpu);
			}
			else 
			{
				cpu.pc() += 3;
			}
		}
	};

	export using jp_z_n16 = jp_cc_n16<is_z_set>;
	export using jp_nz_n16 = jp_cc_n16<is_z_not_set>;
	export using jp_c_n16 = jp_cc_n16<is_c_set>;
	export using jp_nc_n16 = jp_cc_n16<is_c_not_set>;

	export struct jr_n16
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 3_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			const std::uint8_t raw_offset = cpu.memory()[cpu.pc() + 1];
			cpu.pc() += 2 + static_cast<std::int8_t>(raw_offset);
		}
	};

	template <CPUStateCondition condition>
	struct jr_cc_n16
	{
		static constexpr auto num_cycles(const cpu::cpu& cpu) 
		{ 
			return condition::evaluate(cpu) ? 3_m_cycle : 2_m_cycle; 
		}

		static void execute(cpu::cpu& cpu)
		{
			if (condition::evaluate(cpu))
			{
				jr_n16::execute(cpu);
			}
			else 
			{
				cpu.pc() += 2;
			}
		}
	};

	export using jr_z_n16 = jr_cc_n16<is_z_set>;
	export using jr_nz_n16 = jr_cc_n16<is_z_not_set>;
	export using jr_c_n16 = jr_cc_n16<is_c_set>;
	export using jr_nc_n16 = jr_cc_n16<is_c_not_set>;

	export struct jp_hl
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.pc() = cpu.reg().hl();
		}
	};
}