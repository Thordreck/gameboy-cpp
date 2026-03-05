
#include "doctest.h"

import cpu;
import std;
import tests;
import opcodes;

namespace
{
	#define ld_r8_r8_test_cases \
    tests::r8_r8_test_case<opcodes::ld_a_a, tests::get_a, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_a_b, tests::get_a, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_a_c, tests::get_a, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_a_d, tests::get_a, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_a_e, tests::get_a, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_a_h, tests::get_a, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_a_l, tests::get_a, tests::get_l>, \
    tests::r8_r8_test_case<opcodes::ld_b_a, tests::get_b, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_b_b, tests::get_b, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_b_c, tests::get_b, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_b_d, tests::get_b, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_b_e, tests::get_b, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_b_h, tests::get_b, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_b_l, tests::get_b, tests::get_l>, \
    tests::r8_r8_test_case<opcodes::ld_c_a, tests::get_c, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_c_b, tests::get_c, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_c_c, tests::get_c, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_c_d, tests::get_c, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_c_e, tests::get_c, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_c_h, tests::get_c, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_c_l, tests::get_c, tests::get_l>, \
    tests::r8_r8_test_case<opcodes::ld_d_a, tests::get_d, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_d_b, tests::get_d, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_d_c, tests::get_d, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_d_d, tests::get_d, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_d_e, tests::get_d, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_d_h, tests::get_d, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_d_l, tests::get_d, tests::get_l>, \
    tests::r8_r8_test_case<opcodes::ld_e_a, tests::get_e, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_e_b, tests::get_e, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_e_c, tests::get_e, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_e_d, tests::get_e, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_e_e, tests::get_e, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_e_h, tests::get_e, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_e_l, tests::get_e, tests::get_l>, \
    tests::r8_r8_test_case<opcodes::ld_h_a, tests::get_h, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_h_b, tests::get_h, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_h_c, tests::get_h, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_h_d, tests::get_h, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_h_e, tests::get_h, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_h_h, tests::get_h, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_h_l, tests::get_h, tests::get_l>, \
    tests::r8_r8_test_case<opcodes::ld_l_a, tests::get_l, tests::get_a>, \
    tests::r8_r8_test_case<opcodes::ld_l_b, tests::get_l, tests::get_b>, \
    tests::r8_r8_test_case<opcodes::ld_l_c, tests::get_l, tests::get_c>, \
    tests::r8_r8_test_case<opcodes::ld_l_d, tests::get_l, tests::get_d>, \
    tests::r8_r8_test_case<opcodes::ld_l_e, tests::get_l, tests::get_e>, \
    tests::r8_r8_test_case<opcodes::ld_l_h, tests::get_l, tests::get_h>, \
    tests::r8_r8_test_case<opcodes::ld_l_l, tests::get_l, tests::get_l>

	#define ld_r8_n8_test_cases \
    tests::r8_test_case<opcodes::ld_a_n8, tests::get_a>, \
    tests::r8_test_case<opcodes::ld_b_n8, tests::get_b>, \
    tests::r8_test_case<opcodes::ld_c_n8, tests::get_c>, \
    tests::r8_test_case<opcodes::ld_d_n8, tests::get_d>, \
    tests::r8_test_case<opcodes::ld_e_n8, tests::get_e>, \
    tests::r8_test_case<opcodes::ld_h_n8, tests::get_h>, \
    tests::r8_test_case<opcodes::ld_l_n8, tests::get_l>

	#define ld_r16_n16_test_cases \
    tests::r16_test_case<opcodes::ld_bc_n16, tests::get_bc>, \
    tests::r16_test_case<opcodes::ld_de_n16, tests::get_de>, \
    tests::r16_test_case<opcodes::ld_hl_n16, tests::get_hl>

	#define ld_hl_r8_test_cases \
    tests::r8_pos_value_test_case<opcodes::ld_hl_a, tests::get_a, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_hl_b, tests::get_b, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_hl_c, tests::get_c, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_hl_d, tests::get_d, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_hl_e, tests::get_e, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_hl_h, tests::get_h, 0xABCD, 0xAB>, \
    tests::r8_pos_value_test_case<opcodes::ld_hl_l, tests::get_l, 0xABCD, 0xCD>

	#define ld_r8_hl_test_cases \
    tests::r8_pos_value_test_case<opcodes::ld_a_hl, tests::get_a, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_b_hl, tests::get_b, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_c_hl, tests::get_c, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_d_hl, tests::get_d, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_e_hl, tests::get_e, 0xABCD, 0xFA>, \
    tests::r8_pos_value_test_case<opcodes::ld_h_hl, tests::get_h, 0xABCD, 0xAB>, \
    tests::r8_pos_value_test_case<opcodes::ld_l_hl, tests::get_l, 0xABCD, 0xCD>

	#define ld_r16_a_test_cases \
    tests::r16_test_case<opcodes::ld_bc_a, tests::get_bc>, \
    tests::r16_test_case<opcodes::ld_de_a, tests::get_de>

	#define ld_a_r16_test_cases \
    tests::r16_test_case<opcodes::ld_a_bc, tests::get_bc>, \
    tests::r16_test_case<opcodes::ld_a_de, tests::get_de>
}

TEST_CASE_TEMPLATE("load.ld_r8_r8 copies registry value and does not increment pc", Opcode, ld_r8_r8_test_cases)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

	Opcode::rhs(cpu) = test_value;
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::lhs(cpu), test_value);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("load.ld_r8_n8 stores value into target registry and updates pc properly", Opcode, ld_r8_n8_test_cases)
{
	constexpr cpu::register_8::type_t test_value = 0xAB;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.memory().write(0, test_value);
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::reg(cpu), test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE_TEMPLATE("load.ld_r16_n16 stores value into target registry and updates pc properly", Opcode, ld_r16_n16_test_cases)
{
	constexpr cpu::register_16::type_t test_value = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.memory().write(0, static_cast<cpu::register_8::type_t>(test_value));
    cpu.memory().write(1, static_cast<cpu::register_8::type_t>(test_value >> 8));
	Opcode::execute(cpu);

	CHECK_EQ(Opcode::reg(cpu), test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE_TEMPLATE("load.ld_hl_r8 stores value into target registry and does not update pc", test, ld_hl_r8_test_cases)
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().hl() = test::memory_pos;

    test::reg(cpu) = test::target_value;
	test::execute(cpu);

	CHECK_EQ(cpu.memory().read(test::memory_pos), test::target_value);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("load.ld_r8_hl stores value into target registry and does not update pc", test, ld_r8_hl_test_cases)
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().hl() = test::memory_pos;
    memory.bus().write(test::memory_pos, test::target_value);

	test::execute(cpu);

	CHECK_EQ(test::reg(cpu), test::target_value);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ld_a_hli updates a register with value pointed by hl and increments it afterwards")
{
    constexpr memory::memory_address_t test_memory_pos = 0xABCD;
    constexpr memory::memory_data_t test_value = 0xDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    memory.bus().write(test_memory_pos, test_value);
    cpu.reg().hl() = test_memory_pos;

    tests::execute_all_machine_cycles<opcodes::ld_a_hli>(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCE);
    CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ld_a_hld updates a register with value pointed by hl and decrements it afterwards")
{
    constexpr memory::memory_address_t test_memory_pos = 0xABCD;
    constexpr memory::memory_data_t test_value = 0xDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    memory.bus().write(test_memory_pos, test_value);
    cpu.reg().hl() = test_memory_pos;

    tests::execute_all_machine_cycles<opcodes::ld_a_hld>(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCC);
    CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ld_hli_a updates memory pointed by hl with register a's value and increments hl afterwards")
{
    constexpr memory::memory_address_t test_memory_pos = 0xABCD;
    constexpr memory::memory_data_t test_value = 0xDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().a() = test_value;
    cpu.reg().hl() = test_memory_pos;

    tests::execute_all_machine_cycles<opcodes::ld_hli_a>(cpu);

	CHECK_EQ(memory.bus().read(test_memory_pos), test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCE);
    CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ld_hld_a updates memory pointed by hl with register a's value and decrements hl afterwards")
{
    constexpr memory::memory_address_t test_memory_pos = 0xABCD;
    constexpr memory::memory_data_t test_value = 0xDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().a() = test_value;
    cpu.reg().hl() = test_memory_pos;

    tests::execute_all_machine_cycles<opcodes::ld_hld_a>(cpu);

	CHECK_EQ(memory.bus().read(test_memory_pos), test_value);
	CHECK_EQ(cpu.reg().hl(), 0xABCC);
    CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("load.ld_r16_a copies registry a's value into memory pointed by registry", test, ld_r16_a_test_cases)
{
	constexpr memory::memory_data_t test_value = 0xAB;
	constexpr memory::memory_address_t memory_pos = 0xDEFF;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };
    cpu.reg().a() = test_value;

    test::reg(cpu) = memory_pos;
    test::execute(cpu);

	CHECK_EQ(memory.bus().read(memory_pos), test_value);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE_TEMPLATE("load.ld_a_r16 copies value from memory pointed by registry into register a", test, ld_a_r16_test_cases)
{
	constexpr memory::memory_data_t test_value = 0xAB;
	constexpr memory::memory_address_t memory_pos = 0xDEFF;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    memory.bus().write(memory_pos, test_value);
    test::reg(cpu) = memory_pos;
    test::execute(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ld_n16_a copies registry a's value into memory pointed by argument")
{
	constexpr memory::memory_data_t test_value = 0xAB;
	constexpr memory::memory_address_t memory_pos = 0xDEFF;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };
    cpu.reg().a() = test_value;

    cpu.memory().write(0, 0xFF);
    cpu.memory().write(1, 0xDE);

    tests::execute_all_machine_cycles<opcodes::ld_n16_a>(cpu);

	CHECK_EQ(memory.bus().read(memory_pos), test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("load.ld_a_n16 copies value in memory pointed by argument into register a")
{
	constexpr memory::memory_data_t test_value = 0xAB;
	constexpr memory::memory_address_t memory_pos = 0xDEFF;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.memory().write(memory_pos, test_value);
    cpu.memory().write(0, 0xFF);
    cpu.memory().write(1, 0xDE);

    tests::execute_all_machine_cycles<opcodes::ld_a_n16>(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("load.ldh_n16_a copies registry a's value into memory pointed by argument")
{
	constexpr memory::memory_data_t test_value = 0xAB;
	constexpr memory::memory_address_t memory_pos = 0xFFDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().a() = test_value;
    cpu.memory().write(0, 0xDE);

    tests::execute_all_machine_cycles<opcodes::ldh_n16_a>(cpu);

	CHECK_EQ(memory.bus().read(memory_pos), test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("load.ldh_a_n16 copies value pointed by memory into register a")
{
	constexpr memory::memory_data_t test_value = 0xAB;
	constexpr memory::memory_address_t memory_pos = 0xFFDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.memory().write(memory_pos, test_value);
    cpu.memory().write(0, 0xDE);

    tests::execute_all_machine_cycles<opcodes::ldh_a_n16>(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("load.ld_n16_sp updates memory position with sp value")
{
	constexpr memory::memory_address_t test_value = 0xABCD;
	constexpr memory::memory_address_t memory_pos = 0xFFDE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.sp() = test_value;
    memory.bus().write(0, 0xDE);
    memory.bus().write(1, 0xFF);

    tests::execute_all_machine_cycles<opcodes::ld_n16_sp>(cpu);

	CHECK_EQ(memory.bus().read(memory_pos), 0xCD);
	CHECK_EQ(memory.bus().read(memory_pos + 1), 0xAB);
}

TEST_CASE("load.ld_n16_sp updates program counter properly")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    tests::execute_all_machine_cycles<opcodes::ld_n16_sp>(cpu);
	CHECK_EQ(cpu.pc(), 2);
}

TEST_CASE("load.ld_sp_hl updates sp with hl value")
{
	constexpr memory::memory_address_t test_value = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    tests::execute_all_machine_cycles<opcodes::ld_sp_hl>(cpu);
    CHECK_EQ(cpu.sp(), 0);

    cpu.reg().hl() = test_value;
    tests::execute_all_machine_cycles<opcodes::ld_sp_hl>(cpu);
    CHECK_EQ(cpu.sp(), test_value);
}

TEST_CASE("load.ld_sp_hl does not increment program counter")
{
	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    tests::execute_all_machine_cycles<opcodes::ld_sp_hl>(cpu);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ld_hl_n8 stores value into target registry and updates pc properly")
{
    constexpr memory::memory_data_t test_value = 0xFE;
    constexpr memory::memory_address_t memory_pos = 0xABCD;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().hl() = memory_pos;
    memory.bus().write(0, test_value);

	tests::execute_all_machine_cycles<opcodes::ld_hl_n8>(cpu);

	CHECK_EQ(cpu.memory().read(memory_pos), test_value);
	CHECK_EQ(cpu.pc(), 1);
}

TEST_CASE("load.ldh_c_a stores value into target memory and does not increment pc ")
{
    constexpr memory::memory_data_t test_value = 0xFE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().a() = test_value;
    cpu.reg().c() = 0xBD;

	tests::execute_all_machine_cycles<opcodes::ldh_c_a>(cpu);

	CHECK_EQ(memory.bus().read(0xFFBD), test_value);
	CHECK_EQ(cpu.pc(), 0);
}

TEST_CASE("load.ldh_a_c stores value into register a and does not increment pc")
{
    constexpr memory::memory_data_t test_value = 0xFE;

	tests::mock_memory_bus memory{};
	cpu::cpu cpu{ memory.bus() };

    cpu.reg().c() = 0xBD;
    memory.bus().write(0xFFBD, test_value);

	tests::execute_all_machine_cycles<opcodes::ldh_a_c>(cpu);

	CHECK_EQ(cpu.reg().a(), test_value);
	CHECK_EQ(cpu.pc(), 0);
}
