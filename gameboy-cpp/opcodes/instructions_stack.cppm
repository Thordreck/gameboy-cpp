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

	export using pop_af = pop_r16<af_register_provider>;
	export using pop_bc = pop_r16<bc_register_provider>;
	export using pop_de = pop_r16<de_register_provider>;
	export using pop_hl = pop_r16<hl_register_provider>;

	template<ReadOnlyR16RegisterProvider register_provider>
	struct push_r16
	{
		static void execute(cpu::cpu& cpu)
		{
			push_stack(cpu, register_provider::get(cpu));
			cpu.pc()++;
		}
	};

	export using push_af = push_r16<af_readonly_register_provider>;
	export using push_bc = push_r16<bc_readonly_register_provider>;
	export using push_de = push_r16<de_readonly_register_provider>;
	export using push_hl = push_r16<hl_readonly_register_provider>;

	export struct ld_sp_n16
	{
		static void execute(cpu::cpu& cpu)
		{
			cpu.sp() = utils::read_two_byte_little_endian(cpu.memory(), cpu.pc() + 1);
			cpu.pc() += 3;
		}
	};
}