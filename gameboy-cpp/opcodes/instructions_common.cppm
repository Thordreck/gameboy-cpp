
export module opcodes:common;

import cpu;
import std;

namespace opcodes
{
	export template<typename T>
	concept ReadOnlyR8RegisterProvider = requires(const cpu::cpu& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::readonly_register_8>;
	};

	export template<typename T>
	concept R8RegisterProvider = requires(cpu::cpu& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::register_8>;
	};

	export struct a_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.a(); }
	};

	export struct b_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.b(); }
	};

	export struct c_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.c(); }
	};

	export struct d_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.d(); }
	};

	export struct e_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.e(); }
	};

	export struct h_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.h(); }
	};

	export struct l_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.l(); }
	};

	export struct a_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.a(); }
	};

	export struct b_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.b(); }
	};

	export struct c_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.c(); }
	};

	export struct d_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.d(); }
	};

	export struct e_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.e(); }
	};

	export struct h_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.h(); }
	};

	export struct l_readonly_register_provider
	{
		static auto get(const cpu::cpu& cpu) { return cpu.registers.l(); }
	};

	export template<typename T>
	concept R16RegisterProvider = requires(cpu::cpu& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::register_16>;
	};

	export struct af_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.af(); }
	};

	export struct bc_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.bc(); }
	};

	export struct de_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.de(); }
	};

	export struct hl_register_provider
	{
		static auto get(cpu::cpu& cpu) { return cpu.registers.hl(); }
	};
}
