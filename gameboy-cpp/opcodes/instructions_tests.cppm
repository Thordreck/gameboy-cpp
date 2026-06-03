
export module tests;

export import cpu;
export import std;
export import memory;
export import opcodes;

namespace tests
{
	export template<opcodes::Instruction Instruction>
	void execute_all_instruction_steps(cpu::cpu_state& cpu)
	{
		for (std::uint8_t step = 0; step < Instruction::num_steps(cpu); step++)
		{
			Instruction::execute(cpu, step);
		}
	};

	export template<typename Instruction, memory::AnyMemory Memory>
	requires opcodes::ReadOnlyMemoryInstruction<Instruction, Memory>
	void execute_all_instruction_steps(cpu::cpu_state& cpu, const Memory& memory)
	{
		for (std::uint8_t step = 0; step < Instruction::num_steps(cpu); step++)
		{
			Instruction::execute(cpu, step, memory);
		}
	};

	export template<typename Instruction, memory::AnyMemory Memory>
	requires opcodes::MemoryInstruction<Instruction, Memory>
	void execute_all_instruction_steps(cpu::cpu_state& cpu, Memory& memory)
	{
		for (std::uint8_t step = 0; step < Instruction::num_steps(cpu); step++)
		{
			Instruction::execute(cpu, step, memory);
		}
	};

    export template <auto RegFn>
    concept R8RegisterFetchFn = requires(cpu::cpu_state & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_8>;
    };

	export auto get_a(cpu::cpu_state& cpu) { return cpu.reg.a(); }
	export auto get_b(cpu::cpu_state& cpu) { return cpu.reg.b(); }
	export auto get_c(cpu::cpu_state& cpu) { return cpu.reg.c(); }
	export auto get_d(cpu::cpu_state& cpu) { return cpu.reg.d(); }
	export auto get_e(cpu::cpu_state& cpu) { return cpu.reg.e(); }
	export auto get_h(cpu::cpu_state& cpu) { return cpu.reg.h(); }
	export auto get_l(cpu::cpu_state& cpu) { return cpu.reg.l(); }

	export template<opcodes::Instruction OpCode, auto RegFn>
	requires R8RegisterFetchFn<RegFn>
	struct r8_test_case
	{
		static void execute(cpu::cpu_state& cpu) { execute_all_instruction_steps<OpCode>(cpu); }
		static cpu::register_8 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

	export template<typename OpCode, memory::ReadOnlyMemory Memory, auto RegFn>
	requires opcodes::ReadOnlyMemoryInstruction<OpCode, Memory> && R8RegisterFetchFn<RegFn>
	struct r8_read_memory_test_case
	{
		static void execute(cpu::cpu_state& cpu, const Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static cpu::register_8 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

    export template <auto RegFn>
    concept R16RegisterFetchFn = requires(cpu::cpu_state & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_16>;
    };

	export auto get_af(cpu::cpu_state& cpu) { return cpu.reg.af(); }
	export auto get_bc(cpu::cpu_state& cpu) { return cpu.reg.bc(); }
	export auto get_de(cpu::cpu_state& cpu) { return cpu.reg.de(); }
	export auto get_hl(cpu::cpu_state& cpu) { return cpu.reg.hl(); }

	export template<opcodes::Instruction OpCode, auto RegFn>
	requires R16RegisterFetchFn<RegFn>
	struct r16_test_case
	{
		static void execute(cpu::cpu_state& cpu) { execute_all_instruction_steps<OpCode>(cpu); }
		static cpu::register_16 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

	export template<typename OpCode, memory::ReadOnlyMemory Memory, auto RegFn>
	requires opcodes::ReadOnlyMemoryInstruction<OpCode, Memory> && R16RegisterFetchFn<RegFn>
	struct r16_read_memory_test_case
	{
		static void execute(cpu::cpu_state& cpu, const Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static cpu::register_16 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

	export template<typename OpCode, memory::WriteOnlyMemory Memory, auto RegFn>
	requires opcodes::MemoryInstruction<OpCode, Memory> && R16RegisterFetchFn<RegFn>
	struct r16_write_memory_test_case
	{
		static void execute(cpu::cpu_state& cpu, Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static cpu::register_16 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

	export template<typename OpCode, memory::Memory Memory, auto RegFn>
	requires opcodes::MemoryInstruction<OpCode, Memory> && R16RegisterFetchFn<RegFn>
	struct r16_memory_test_case
	{
		static void execute(cpu::cpu_state& cpu, Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static cpu::register_16 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

	export template<opcodes::Instruction OpCode, auto RegFn, std::uint8_t BitIndex>
	requires R8RegisterFetchFn<RegFn> && (BitIndex <= 7)
	struct r8_u3_test_case
	{
		static constexpr auto bit_index = BitIndex;

		static void execute(cpu::cpu_state& cpu) { execute_all_instruction_steps<OpCode>(cpu); }
		static cpu::register_8 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

    export template <auto FlagRegFn>
    concept FlagRegisterFetchFn = requires(cpu::cpu_state & cpu)
    {
        { FlagRegFn(cpu) } -> std::convertible_to<cpu::flag_register>;
    };

	export auto get_z_flag(cpu::cpu_state& cpu) { return cpu.reg.z_flag(); }
	export auto get_c_flag(cpu::cpu_state& cpu) { return cpu.reg.c_flag(); }

	export template <auto FlagRegFn, bool value>
	requires FlagRegisterFetchFn<FlagRegFn>
	struct flag_setter
	{
		static void set(cpu::cpu_state& cpu)
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
	concept CpuStateConditionSetter = requires(cpu::cpu_state & cpu)
	{
		{ T::set(cpu) } -> std::same_as<void>;
	};

	export template<typename OpCode, memory::Memory Memory, CpuStateConditionSetter condition_setter>
	requires opcodes::MemoryInstruction<OpCode, Memory>
	struct cc_test_case
	{
		static void execute(cpu::cpu_state& cpu, Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static void set_condition(cpu::cpu_state& cpu) { condition_setter::set(cpu); }
	};

	export template<opcodes::Instruction OpCode, auto LhsRegFn, auto RhsRegFn>
    requires R8RegisterFetchFn<LhsRegFn> && R8RegisterFetchFn<RhsRegFn>
	struct r8_r8_test_case
	{
		static void execute(cpu::cpu_state& cpu) { execute_all_instruction_steps<OpCode>(cpu); }
		static cpu::register_8 lhs(cpu::cpu_state& cpu) { return LhsRegFn(cpu); }
		static cpu::register_8 rhs(cpu::cpu_state& cpu) { return RhsRegFn(cpu); }
	};

	export template<typename OpCode, memory::Memory Memory, auto RegFn, cpu::register_16::type_t memory_pos, cpu::register_8::type_t target_value>
    requires opcodes::MemoryInstruction<OpCode, Memory> && R8RegisterFetchFn<RegFn>
	struct r8_memory_pos_value_test_case
	{
        static constexpr auto memory_pos = memory_pos;
        static constexpr auto target_value = target_value;

		static void execute(cpu::cpu_state& cpu, Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static cpu::register_8 reg(cpu::cpu_state& cpu) { return RegFn(cpu); }
	};

	export template<typename OpCode, memory::WriteOnlyMemory Memory, std::uint8_t vec>
	requires opcodes::MemoryInstruction<OpCode, Memory> && opcodes::RSTVector<vec>
	struct rst_vec_test_case
	{
		static void execute(cpu::cpu_state& cpu, Memory& memory) { execute_all_instruction_steps<OpCode>(cpu, memory); }
		static constexpr auto vector = vec;
	};

	export class test_memory
	{
	public:
		[[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const { return memory[address]; }
		void write(const memory::memory_address_t address, const memory::memory_data_t value) { memory[address] = value; }

	private:
		std::array<memory::memory_data_t, memory::memory_size> memory {};
	};

}
