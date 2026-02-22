export module opcodes:misc;

import cpu;

namespace opcodes
{
	using namespace cpu::literals;

	export struct nop
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			cpu.pc()++;
		}
	};

	export struct daa
	{
		static constexpr auto num_cycles(const cpu::cpu&) { return 1_m_cycle; }

		static void execute(cpu::cpu& cpu)
		{
			bool should_carry = false;
			std::uint8_t adjustment = 0x0;
			
			const std::uint8_t a = cpu.reg().a();
			const bool half_carry = cpu.reg().h_flag();
			const bool carry = cpu.reg().c_flag();
			const bool substract = cpu.reg().n_flag();

			if ((!substract && (a & 0xF) > 0x09) || half_carry)
			{
				adjustment |= 0x06;
			}

			if ((!substract && a > 0x99) || carry)
			{
				adjustment |= 0x60;
				should_carry = true;
			}

			cpu.reg().a() = substract
				? cpu.reg().a() - adjustment
				: cpu.reg().a() + adjustment;

			cpu.reg().z_flag() = cpu.reg().a() == 0;
			cpu.reg().h_flag() = false;
			cpu.reg().c_flag() = should_carry;

			cpu.pc()++;
		}
	};
}
