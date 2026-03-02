export module opcodes:subroutine;

import std;
import cpu;
import utilities;

import :common;
import :jump;
import :stack;

namespace opcodes
{
	using namespace cpu::literals;

	export struct call_n16
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 6_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				using namespace literals;
				cpu.cache().r16 = cpu.pc() + 2_u16;

				cpu.memory()[--cpu.sp()] = utils::most_significant_byte(cpu.cache().r16);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				cpu.memory()[--cpu.sp()] = utils::less_significant_byte(cpu.cache().r16);
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory()[cpu.pc()++];
			}
			else if (cpu::is_end_of_machine_cycle<4>(cpu.cycle()))
			{
				const std::uint8_t low_byte = cpu.cache().r8;
				const std::uint8_t high_byte = cpu.memory()[cpu.pc()++];

				cpu.cache().r16 = utils::encode_little_endian(low_byte, high_byte);
			}
			else if (cpu::is_end_of_machine_cycle<5>(cpu.cycle()))
			{
				cpu.pc() = cpu.cache().r16;
			}
		}
	};

	template<CPUStateCondition condition>
	struct call_cc_n16
	{
		static constexpr auto num_cycles(const cpu::cpu& cpu) 
		{
			return condition::evaluate(cpu) ? 6_m_cycle : 3_m_cycle; 
		}

		static void execute(cpu::cpu& cpu)
		{
			if (condition::evaluate(cpu))
			{
				call_n16::execute(cpu);
			}
			else if(cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				cpu.pc() += 2;
			}
		}
	};

	export using call_z_n16 = call_cc_n16<is_z_set>;
	export using call_nz_n16 = call_cc_n16<is_z_not_set>;
	export using call_c_n16 = call_cc_n16<is_c_set>;
	export using call_nc_n16 = call_cc_n16<is_c_not_set>;

	export struct ret
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory()[cpu.sp()++];
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				const std::uint8_t less_significant = cpu.cache().r8;
				const std::uint8_t most_significant = cpu.memory()[cpu.sp()++];

				cpu.cache().r16 = utils::encode_little_endian(less_significant, most_significant);
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				cpu.pc() = cpu.cache().r16;
			}
		}
	};

	template<CPUStateCondition condition>
	struct ret_cc
	{
		static constexpr auto num_cycles(const cpu::cpu& cpu) 
		{ 
			return condition::evaluate(cpu) ? 5_m_cycle : 2_m_cycle; 
		}

		static void execute(cpu::cpu& cpu)
		{
			if (condition::evaluate(cpu))
			{
				ret::execute(cpu);
			}
		}
	};

	export using ret_z = ret_cc<is_z_set>;
	export using ret_nz = ret_cc<is_z_not_set>;
	export using ret_c = ret_cc<is_c_set>;
	export using ret_nc = ret_cc<is_c_not_set>;

	export struct reti
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.ime_flag().enable();
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				cpu.cache().r8 = cpu.memory()[cpu.sp()++];
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				const std::uint8_t less_significant = cpu.cache().r8;
				const std::uint8_t most_significant = cpu.memory()[cpu.sp()++];

				cpu.pc() = utils::encode_little_endian(less_significant, most_significant);
			}
		}
	};

	template<std::uint8_t vec>
	requires RSTVector<vec>
	struct rst_vec
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 4_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			if (cpu::is_end_of_machine_cycle<0>(cpu.cycle()))
			{
				using namespace literals;
				cpu.cache().r16 = cpu.pc();
			}
			else if (cpu::is_end_of_machine_cycle<1>(cpu.cycle()))
			{
				cpu.memory()[--cpu.sp()] = utils::most_significant_byte(cpu.cache().r16);
			}
			else if (cpu::is_end_of_machine_cycle<2>(cpu.cycle()))
			{
				cpu.memory()[--cpu.sp()] = utils::less_significant_byte(cpu.cache().r16);
			}
			else if (cpu::is_end_of_machine_cycle<3>(cpu.cycle()))
			{
				cpu.pc() = vec;
			}
		}
	};

	export using rst_00 = rst_vec <0x00>;
	export using rst_08 = rst_vec <0x08>;
	export using rst_10 = rst_vec <0x10>;
	export using rst_18 = rst_vec <0x18>;
	export using rst_20 = rst_vec <0x20>;
	export using rst_28 = rst_vec <0x28>;
	export using rst_30 = rst_vec <0x30>;
	export using rst_38 = rst_vec <0x38>;
}