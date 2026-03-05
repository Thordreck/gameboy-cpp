
export module tests;

export import cpu;
export import std;
export import memory;
export import opcodes;

namespace tests
{
	export template<opcodes::Instruction instruction>
	void execute_all_machine_cycles(cpu::cpu& cpu)
	{
		cpu.cycle() = {};

		while (cpu.cycle().m_cycle() < instruction::num_cycles(cpu))
		{
			instruction::execute(cpu);
			cpu.cycle()++;
		}
	};

    export template <auto RegFn>
    concept R8RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_8>;
    };

	export auto get_a(cpu::cpu& cpu) { return cpu.reg().a(); }
	export auto get_b(cpu::cpu& cpu) { return cpu.reg().b(); }
	export auto get_c(cpu::cpu& cpu) { return cpu.reg().c(); }
	export auto get_d(cpu::cpu& cpu) { return cpu.reg().d(); }
	export auto get_e(cpu::cpu& cpu) { return cpu.reg().e(); }
	export auto get_h(cpu::cpu& cpu) { return cpu.reg().h(); }
	export auto get_l(cpu::cpu& cpu) { return cpu.reg().l(); }

	export template<opcodes::Instruction OpCode, auto RegFn>
	requires R8RegisterFetchFn<RegFn>
	struct r8_test_case
	{
		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

    export template <auto RegFn>
    concept R16RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_16>;
    };

	export auto get_af(cpu::cpu& cpu) { return cpu.reg().af(); }
	export auto get_bc(cpu::cpu& cpu) { return cpu.reg().bc(); }
	export auto get_de(cpu::cpu& cpu) { return cpu.reg().de(); }
	export auto get_hl(cpu::cpu& cpu) { return cpu.reg().hl(); }

	export template<opcodes::Instruction OpCode, auto RegFn>
	requires R16RegisterFetchFn<RegFn>
	struct r16_test_case
	{
		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static cpu::register_16 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	export template<opcodes::Instruction OpCode, auto RegFn, std::uint8_t BitIndex>
	requires R8RegisterFetchFn<RegFn> && (BitIndex <= 7)
	struct r8_u3_test_case
	{
		static constexpr auto bit_index = BitIndex;

		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

    export template <auto FlagRegFn>
    concept FlagRegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { FlagRegFn(cpu) } -> std::convertible_to<cpu::flag_register>;
    };

	export auto get_z_flag(cpu::cpu& cpu) { return cpu.reg().z_flag(); }
	export auto get_c_flag(cpu::cpu& cpu) { return cpu.reg().c_flag(); }

	export template <auto FlagRegFn, bool value>
	requires FlagRegisterFetchFn<FlagRegFn>
	struct flag_setter
	{
		static void set(cpu::cpu& cpu)
		{
			FlagRegFn(cpu) = value;
		}
	};

	export template <auto FlagRegFn>
	using set_flag = flag_setter<FlagRegFn, true>;

	export template <auto FlagRegFn>
	using unset_flag = flag_setter<FlagRegFn, false>;

	export using set_z_flag = set_flag<get_z_flag>;
	export using unset_z_flag = unset_flag<get_z_flag>;

	export using set_c_flag = set_flag<get_c_flag>;
	export using unset_c_flag = unset_flag<get_c_flag>;

	export template <typename T>
	concept CpuStateConditionSetter = requires(cpu::cpu & cpu)
	{
		{ T::set(cpu) } -> std::same_as<void>;
	};

	export template<opcodes::Instruction OpCode, CpuStateConditionSetter condition_setter>
	struct cc_test_case
	{
		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static void set_condition(cpu::cpu& cpu) { condition_setter::set(cpu); }
	};

	export template<opcodes::Instruction OpCode, auto LhsRegFn, auto RhsRegFn>
    requires R8RegisterFetchFn<LhsRegFn> && R8RegisterFetchFn<RhsRegFn>
	struct r8_r8_test_case
	{
		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static cpu::register_8 lhs(cpu::cpu& cpu) { return LhsRegFn(cpu); }
		static cpu::register_8 rhs(cpu::cpu& cpu) { return RhsRegFn(cpu); }
	};

	export template<opcodes::Instruction OpCode, auto RegFn, cpu::register_16::type_t memory_pos, cpu::register_8::type_t target_value>
    requires R8RegisterFetchFn<RegFn>
	struct r8_pos_value_test_case
	{
        static constexpr auto memory_pos = memory_pos;
        static constexpr auto target_value = target_value;

		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	export template<opcodes::Instruction OpCode, std::uint8_t vec>
	requires opcodes::RSTVector<vec>
	struct rst_vec_test_case
	{
		static void execute(cpu::cpu& cpu) { execute_all_machine_cycles<OpCode>(cpu); }
		static constexpr auto vector = vec;
	};

	export class mock_memory_bus
	{
	public:
		mock_memory_bus()
			: memory {}
			, memory_region{ memory }
			, memory_map{ memory::build_memory_map(memory_region) }
			, memory_bus{ memory_map }
		{}

		memory::memory_bus& bus() { return memory_bus; }

	private:
		std::array<memory::memory_data_t, memory::memory_size> memory;
		memory::span_map<memory::memory_size> memory_region;
		memory::memory_map_array_t memory_map;
		memory::memory_bus memory_bus;
	};
}
