export module opcodes:misc;

import cpu;

namespace opcodes
{
	export struct nop
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.pc()++;
		}
	};

	export struct daa
	{
		static void execute(cpu::cpu& cpu)
		{
			bool carry = false;

			if (cpu.reg().n_flag())
			{
				cpu::register_8::type_t adjustment {};

				if (cpu.reg().h_flag())
				{
					adjustment |= 0x06;
				}

				if (cpu.reg().c_flag())
				{
					adjustment |= 0x60;
				}

				cpu.reg().a() = cpu.reg().a() - adjustment;
			}
			else 
			{
				cpu::register_8::type_t adjustment {};

				if (cpu.reg().h_flag() || ((cpu.reg().a() & 0x0F) > 0x09))
				{
					adjustment |= 0x06;
				}

				if (cpu.reg().c_flag() || cpu.reg().a() > 0x99)
				{
					adjustment |= 0x60;
					carry = true;
				}

				cpu.reg().a() = cpu.reg().a() + adjustment;
			}

			cpu.reg().z_flag() = cpu.reg().a() == 0;
			cpu.reg().h_flag() = false;
			cpu.reg().c_flag() = carry;

			cpu.pc()++;
		}
	};
}
