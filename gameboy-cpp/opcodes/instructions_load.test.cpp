
#include "doctest.h"

import cpu;
import std;
import opcodes;

namespace
{
    template <auto RegFn>
    concept R8RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_8>;
    };

	auto get_a(cpu::cpu& cpu) { return cpu.reg().a(); }
	auto get_b(cpu::cpu& cpu) { return cpu.reg().b(); }
	auto get_c(cpu::cpu& cpu) { return cpu.reg().c(); }
	auto get_d(cpu::cpu& cpu) { return cpu.reg().d(); }
	auto get_e(cpu::cpu& cpu) { return cpu.reg().e(); }
	auto get_h(cpu::cpu& cpu) { return cpu.reg().h(); }
	auto get_l(cpu::cpu& cpu) { return cpu.reg().l(); }

    template <auto RegFn>
    concept R16RegisterFetchFn = requires(cpu::cpu & cpu)
    {
        { RegFn(cpu) } -> std::convertible_to<cpu::register_16>;
    };

	auto get_af(cpu::cpu& cpu) { return cpu.reg().af(); }
	auto get_bc(cpu::cpu& cpu) { return cpu.reg().bc(); }
	auto get_de(cpu::cpu& cpu) { return cpu.reg().de(); }
	auto get_hl(cpu::cpu& cpu) { return cpu.reg().hl(); }

	template<opcodes::Instruction OpCode, auto RegFn>
    requires R8RegisterFetchFn<RegFn>
	struct r8_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	template<opcodes::Instruction OpCode, auto LhsRegFn, auto RhsRegFn>
    requires R8RegisterFetchFn<LhsRegFn> && R8RegisterFetchFn<RhsRegFn>
	struct r8_r8_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_8 lhs(cpu::cpu& cpu) { return LhsRegFn(cpu); }
		static cpu::register_8 rhs(cpu::cpu& cpu) { return RhsRegFn(cpu); }
	};

	template<opcodes::Instruction OpCode, auto RegFn>
    requires R16RegisterFetchFn<RegFn>
	struct r16_test_case
	{
		static constexpr auto execute = OpCode::execute;
		static cpu::register_16 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	template<opcodes::Instruction OpCode, auto RegFn, cpu::register_16::type_t memory_pos, cpu::register_8::type_t target_value>
    requires R8RegisterFetchFn<RegFn>
	struct r8_pos_value_test_case
	{
		static constexpr auto execute = OpCode::execute;
        static constexpr auto memory_pos = memory_pos;
        static constexpr auto target_value = target_value;
		static cpu::register_8 reg(cpu::cpu& cpu) { return RegFn(cpu); }
	};

	#define ld_r8_r8_test_cases \
    r8_r8_test_case<opcodes::ld_a_a, get_a, get_a>, \
    r8_r8_test_case<opcodes::ld_a_b, get_a, get_b>, \
    r8_r8_test_case<opcodes::ld_a_c, get_a, get_c>, \
    r8_r8_test_case<opcodes::ld_a_d, get_a, get_d>, \
    r8_r8_test_case<opcodes::ld_a_e, get_a, get_e>, \
    r8_r8_test_case<opcodes::ld_a_h, get_a, get_h>, \
    r8_r8_test_case<opcodes::ld_a_l, get_a, get_l>, \
    r8_r8_test_case<opcodes::ld_b_a, get_b, get_a>, \
    r8_r8_test_case<opcodes::ld_b_b, get_b, get_b>, \
    r8_r8_test_case<opcodes::ld_b_c, get_b, get_c>, \
    r8_r8_test_case<opcodes::ld_b_d, get_b, get_d>, \
    r8_r8_test_case<opcodes::ld_b_e, get_b, get_e>, \
    r8_r8_test_case<opcodes::ld_b_h, get_b, get_h>, \
    r8_r8_test_case<opcodes::ld_b_l, get_b, get_l>, \
    r8_r8_test_case<opcodes::ld_c_a, get_c, get_a>, \
    r8_r8_test_case<opcodes::ld_c_b, get_c, get_b>, \
    r8_r8_test_case<opcodes::ld_c_c, get_c, get_c>, \
    r8_r8_test_case<opcodes::ld_c_d, get_c, get_d>, \
    r8_r8_test_case<opcodes::ld_c_e, get_c, get_e>, \
    r8_r8_test_case<opcodes::ld_c_h, get_c, get_h>, \
    r8_r8_test_case<opcodes::ld_c_l, get_c, get_l>, \
    r8_r8_test_case<opcodes::ld_d_a, get_d, get_a>, \
    r8_r8_test_case<opcodes::ld_d_b, get_d, get_b>, \
    r8_r8_test_case<opcodes::ld_d_c, get_d, get_c>, \
    r8_r8_test_case<opcodes::ld_d_d, get_d, get_d>, \
    r8_r8_test_case<opcodes::ld_d_e, get_d, get_e>, \
    r8_r8_test_case<opcodes::ld_d_h, get_d, get_h>, \
    r8_r8_test_case<opcodes::ld_d_l, get_d, get_l>, \
    r8_r8_test_case<opcodes::ld_e_a, get_e, get_a>, \
    r8_r8_test_case<opcodes::ld_e_b, get_e, get_b>, \
    r8_r8_test_case<opcodes::ld_e_c, get_e, get_c>, \
    r8_r8_test_case<opcodes::ld_e_d, get_e, get_d>, \
    r8_r8_test_case<opcodes::ld_e_e, get_e, get_e>, \
    r8_r8_test_case<opcodes::ld_e_h, get_e, get_h>, \
    r8_r8_test_case<opcodes::ld_e_l, get_e, get_l>, \
    r8_r8_test_case<opcodes::ld_h_a, get_h, get_a>, \
    r8_r8_test_case<opcodes::ld_h_b, get_h, get_b>, \
    r8_r8_test_case<opcodes::ld_h_c, get_h, get_c>, \
    r8_r8_test_case<opcodes::ld_h_d, get_h, get_d>, \
    r8_r8_test_case<opcodes::ld_h_e, get_h, get_e>, \
    r8_r8_test_case<opcodes::ld_h_h, get_h, get_h>, \
    r8_r8_test_case<opcodes::ld_h_l, get_h, get_l>, \
    r8_r8_test_case<opcodes::ld_l_a, get_l, get_a>, \
    r8_r8_test_case<opcodes::ld_l_b, get_l, get_b>, \
    r8_r8_test_case<opcodes::ld_l_c, get_l, get_c>, \
    r8_r8_test_case<opcodes::ld_l_d, get_l, get_d>, \
    r8_r8_test_case<opcodes::ld_l_e, get_l, get_e>, \
    r8_r8_test_case<opcodes::ld_l_h, get_l, get_h>, \
    r8_r8_test_case<opcodes::ld_l_l, get_l, get_l>

	#define ld_r8_n8_test_cases \
    r8_test_case<opcodes::ld_a_n8, get_a>, \
    r8_test_case<opcodes::ld_b_n8, get_b>, \
    r8_test_case<opcodes::ld_c_n8, get_c>, \
    r8_test_case<opcodes::ld_d_n8, get_d>, \
    r8_test_case<opcodes::ld_e_n8, get_e>, \
    r8_test_case<opcodes::ld_h_n8, get_h>, \
    r8_test_case<opcodes::ld_l_n8, get_l>

	#define ld_r16_n16_test_cases \
    r16_test_case<opcodes::ld_bc_n16, get_bc>, \
    r16_test_case<opcodes::ld_de_n16, get_de>, \
    r16_test_case<opcodes::ld_hl_n16, get_hl>

	#define ld_hl_r8_test_cases \
    r8_pos_value_test_case<opcodes::ld_hl_a, get_a, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_hl_b, get_b, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_hl_c, get_c, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_hl_d, get_d, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_hl_e, get_e, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_hl_h, get_h, 0xABCD, 0xAB>, \
    r8_pos_value_test_case<opcodes::ld_hl_l, get_l, 0xABCD, 0xCD>

	#define ld_r8_hl_test_cases \
    r8_pos_value_test_case<opcodes::ld_a_hl, get_a, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_b_hl, get_b, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_c_hl, get_c, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_d_hl, get_d, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_e_hl, get_e, 0xABCD, 0xFA>, \
    r8_pos_value_test_case<opcodes::ld_h_hl, get_h, 0xABCD, 0xAB>, \
    r8_pos_value_test_case<opcodes::ld_l_hl, get_l, 0xABCD, 0xCD>

	#define ld_r16_a_test_cases \
    r16_test_case<opcodes::ld_bc_a, get_bc>, \
    r16_test_case<opcodes::ld_de_a, get_de>

	#define ld_a_r16_test_cases \
    r16_test_case<opcodes::ld_a_bc, get_bc>, \
    r16_test_case<opcodes::ld_a_de, get_de>
}

TEST_CASE_TEMPLATE("ld_r8_r8 copies registry value and updates pc properly", Opcode, ld_r8_r8_test_cases)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	Opcode::rhs(cpu) = test_value;
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::lhs(cpu), test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("ld_r8_n8 stores value into target registry and updates pc properly", Opcode, ld_r8_n8_test_cases)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.memory()[1] = test_value;
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::reg(cpu), test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("ld_r16_n16 stores value into target registry and updates pc properly", Opcode, ld_r16_n16_test_cases)
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.memory()[1] = static_cast<cpu::register_8::type_t>(test_value);
    cpu.memory()[2] = static_cast<cpu::register_8::type_t>(test_value >> 8);
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::reg(cpu), test_value);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE_TEMPLATE("ld_hl_r8 stores value into target registry and updates pc properly", test, ld_hl_r8_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.reg().hl() = test::memory_pos;

    test::reg(cpu) = test::target_value;
	test::execute(cpu);

	CHECK_EQ(cpu.memory()[test::memory_pos], test::target_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("ld_r8_hl stores value into target registry and updates pc properly", test, ld_r8_hl_test_cases)
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.reg().hl() = test::memory_pos;
    memory[test::memory_pos] = test::target_value;

	test::execute(cpu);

	CHECK_EQ(test::reg(cpu), test::target_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("ld_a_hli updates a register with value pointed by hl and increments it afterwards")
{
    constexpr cpu::memory_bus::index_t test_memory_pos = 0xABCD;
    constexpr cpu::memory_bus::type_t test_value = 0xDE;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    memory[test_memory_pos] = test_value;
    cpu.reg().hl() = test_memory_pos;

    opcodes::ld_a_hli::execute(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCE);
    CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("ld_a_hld updates a register with value pointed by hl and decrements it afterwards")
{
    constexpr cpu::memory_bus::index_t test_memory_pos = 0xABCD;
    constexpr cpu::memory_bus::type_t test_value = 0xDE;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    memory[test_memory_pos] = test_value;
    cpu.reg().hl() = test_memory_pos;

    opcodes::ld_a_hld::execute(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCC);
    CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("ld_hli_a updates memory pointed by hl with register a's value and increments hl afterwards")
{
    constexpr cpu::memory_bus::index_t test_memory_pos = 0xABCD;
    constexpr cpu::memory_bus::type_t test_value = 0xDE;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.reg().a() = test_value;
    cpu.reg().hl() = test_memory_pos;

    opcodes::ld_hli_a::execute(cpu);

	CHECK_EQ(memory[test_memory_pos], test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCE);
    CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("ld_hld_a updates memory pointed by hl with register a's value and decrements hl afterwards")
{
    constexpr cpu::memory_bus::index_t test_memory_pos = 0xABCD;
    constexpr cpu::memory_bus::type_t test_value = 0xDE;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.reg().a() = test_value;
    cpu.reg().hl() = test_memory_pos;

    opcodes::ld_hld_a::execute(cpu);

	CHECK_EQ(memory[test_memory_pos], test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCC);
    CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("ld_r16_a copies registry a's value into memory pointed by registry", test, ld_r16_a_test_cases)
{
	constexpr cpu::memory_bus::type_t test_value = 0xAB;
	constexpr cpu::memory_bus::index_t memory_pos = 0xDEFF;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
    cpu.reg().a() = test_value;

    test::reg(cpu) = memory_pos;
    test::execute(cpu);

	CHECK_EQ(memory[memory_pos], test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("ld_a_r16 copies value from memory pointed by registry into register a", test, ld_a_r16_test_cases)
{
	constexpr cpu::memory_bus::type_t test_value = 0xAB;
	constexpr cpu::memory_bus::index_t memory_pos = 0xDEFF;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    memory[memory_pos] = test_value;
    test::reg(cpu) = memory_pos;
    test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("ld_n16_a copies registry a's value into memory pointed by argument")
{
	constexpr cpu::memory_bus::type_t test_value = 0xAB;
	constexpr cpu::memory_bus::index_t memory_pos = 0xDEFF;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
    cpu.reg().a() = test_value;

    cpu.memory()[1] = 0xFF;
    cpu.memory()[2] = 0xDE;

    opcodes::ld_n16_a::execute(cpu);

	CHECK_EQ(memory[memory_pos], test_value);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("ld_a_n16 copies value in memory pointed by argument into register a")
{
	constexpr cpu::memory_bus::type_t test_value = 0xAB;
	constexpr cpu::memory_bus::index_t memory_pos = 0xDEFF;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.memory()[memory_pos] = test_value;
    cpu.memory()[1] = 0xFF;
    cpu.memory()[2] = 0xDE;

    opcodes::ld_a_n16::execute(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("ldh_n16_a copies registry a's value into memory pointed by argument")
{
	constexpr cpu::memory_bus::type_t test_value = 0xAB;
	constexpr cpu::memory_bus::index_t memory_pos = 0xFFDE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.reg().a() = test_value;
    cpu.memory()[1] = 0xDE;

    opcodes::ldh_n16_a::execute(cpu);

	CHECK_EQ(memory[memory_pos], test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("ldh_a_n16 copies value pointed by memory into register a")
{
	constexpr cpu::memory_bus::type_t test_value = 0xAB;
	constexpr cpu::memory_bus::index_t memory_pos = 0xFFDE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.memory()[memory_pos] = test_value;
    cpu.memory()[1] = 0xDE;

    opcodes::ldh_a_n16::execute(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("ld_n16_sp updates memory position with sp value")
{
	constexpr cpu::memory_bus::index_t test_value = 0xABCD;
	constexpr cpu::memory_bus::index_t memory_pos = 0xFFDE;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    cpu.sp() = test_value;
    memory[1] = 0xDE;
    memory[2] = 0xFF;

    opcodes::ld_n16_sp::execute(cpu);

	CHECK_EQ(memory[memory_pos], 0xCD);
	CHECK_EQ(memory[memory_pos + 1], 0xAB);
}

TEST_CASE("ld_n16_sp updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    opcodes::ld_n16_sp::execute(cpu);
	CHECK_EQ(cpu.pc(), 3);
}

TEST_CASE("ld_sp_hl updates sp with hl value")
{
	constexpr cpu::memory_bus::index_t test_value = 0xABCD;

	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    opcodes::ld_sp_hl::execute(cpu);
    CHECK_EQ(cpu.sp(), 0);

    cpu.reg().hl() = test_value;
    opcodes::ld_sp_hl::execute(cpu);
    CHECK_EQ(cpu.sp(), test_value);
}

TEST_CASE("ld_sp_hl updates program counter properly")
{
	std::array<std::uint8_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

    opcodes::ld_sp_hl::execute(cpu);
	CHECK_EQ(cpu.pc(), 1);
}
