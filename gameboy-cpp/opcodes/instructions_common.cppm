
export module opcodes:common;

export import std;
export import cpu;

namespace opcodes
{
	export using opcode_t = std::uint8_t;
	export using step_t = std::uint8_t;

	export template<typename T>
	concept Instruction = requires(cpu::cpu_state& cpu, const step_t step)
	{
		{ T::execute(cpu, step) } -> std::same_as<void>;
		{ T::num_steps(cpu) } -> std::convertible_to<step_t>;
	};

	export constexpr opcode_t prefix_opcode = 0xCB;

	export template<typename T>
	concept ReadOnlyR8RegisterProvider = requires(const cpu::cpu_state& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::readonly_register_8>;
	};

	export template<typename T>
	concept R8RegisterProvider = requires(cpu::cpu_state& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::register_8>;
	};

	export struct a_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.a(); }
	};

	export struct b_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.b(); }
	};

	export struct c_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.c(); }
	};

	export struct d_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.d(); }
	};

	export struct e_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.e(); }
	};

	export struct h_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.h(); }
	};

	export struct l_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.l(); }
	};

	export struct a_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.a(); }
	};

	export struct b_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.b(); }
	};

	export struct c_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.c(); }
	};

	export struct d_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.d(); }
	};

	export struct e_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.e(); }
	};

	export struct h_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.h(); }
	};

	export struct l_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.l(); }
	};

	export template<typename T>
	concept R16RegisterProvider = requires(cpu::cpu_state& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::register_16>;
	};

	export struct af_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.af(); }
	};

	export struct bc_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.bc(); }
	};

	export struct de_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.de(); }
	};

	export struct hl_register_provider
	{
		static auto get(cpu::cpu_state& cpu) { return cpu.reg.hl(); }
	};

	export template<typename T>
	concept ReadOnlyR16RegisterProvider = requires(const cpu::cpu_state& cpu)
	{
		{ T::get(cpu) } -> std::convertible_to<cpu::readonly_register_16>;
	};

	export struct af_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.af(); }
	};

	export struct bc_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.bc(); }
	};

	export struct de_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.de(); }
	};

	export struct hl_readonly_register_provider
	{
		static auto get(const cpu::cpu_state& cpu) { return cpu.reg.hl(); }
	};

	export template<typename T>
	concept CPUStateCondition = requires(const cpu::cpu_state& cpu)
	{
		{ T::evaluate(cpu) } -> std::convertible_to<bool>;
	};

	export struct is_z_set
	{
		static bool evaluate(const cpu::cpu_state& cpu) 
		{ 
			return cpu.reg.z_flag();
		}
	};

	export struct is_z_not_set
	{
		static bool evaluate(const cpu::cpu_state& cpu)
		{
			return !is_z_set::evaluate(cpu);
		}
	};

	export struct is_c_set
	{
		static bool evaluate(const cpu::cpu_state& cpu)
		{
			return cpu.reg.c_flag();
		}
	};

	export struct is_c_not_set
	{
		static bool evaluate(const cpu::cpu_state& cpu)
		{
			return !is_c_set::evaluate(cpu);
		}
	};

	export template<auto T>
	concept BitIndex = std::unsigned_integral<decltype(T)> && (T <= 7);

	export template<std::uint8_t Vec>
	concept RSTVector =
		Vec == 0x00 || Vec == 0x08 ||
		Vec == 0x10 || Vec == 0x18 ||
		Vec == 0x20 || Vec == 0x28 ||
		Vec == 0x30 || Vec == 0x38;
}

namespace opcodes::literals
{
	consteval std::uint8_t operator"" _u8(unsigned long long value)
	{
		return static_cast<std::uint8_t>(value);
	}

	consteval std::uint16_t operator"" _u16(unsigned long long value)
	{
		return static_cast<std::uint16_t>(value);
	}
}
