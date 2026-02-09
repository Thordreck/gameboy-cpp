
#include "doctest.h"

import cpu;
import std;

TEST_CASE("add_a_r8 updates a register properly")
{
	cpu::registers registers{};
	cpu::add_a_r8 instruction{ registers.a(), registers.b(), registers.flags()};

	instruction.execute();
	CHECK_EQ(registers.a(), 0);

	registers.b() = 5;
	instruction.execute();
	CHECK_EQ(registers.a(), 5);

	registers.b() = 125;
	instruction.execute();
	CHECK_EQ(registers.a(), 130);

	registers.b() = 127;
	instruction.execute();
	CHECK_EQ(registers.a(), 1);
}

TEST_CASE("add_a_r8 updates zero flag properly")
{
	cpu::registers registers{};
	cpu::add_a_r8 instruction{ registers.a(), registers.b(), registers.flags()};

	instruction.execute();
	CHECK(registers.z_flag());

	registers.b() = 5;
	instruction.execute();
	CHECK_FALSE(registers.z_flag());

	registers.b() = 251;
	instruction.execute();
	CHECK(registers.z_flag());
}

TEST_CASE("add_a_r8 always set substraction flag to zero")
{
	cpu::registers registers{};
	cpu::add_a_r8 instruction{ registers.a(), registers.b(), registers.flags()};

	instruction.execute();
	CHECK_FALSE(registers.n_flag());

	registers.b() = 5;
	instruction.execute();
	CHECK_FALSE(registers.n_flag());

	registers.b() = 252;
	instruction.execute();
	CHECK_FALSE(registers.n_flag());
}

TEST_CASE("add_a_r8 applies carry flag properly")
{
	cpu::registers registers{};
	cpu::add_a_r8 instruction{ registers.a(), registers.b(), registers.flags()};

	instruction.execute();
	CHECK_FALSE(registers.c_flag());

	registers.b() = 255;
	instruction.execute();
	CHECK_FALSE(registers.c_flag());

	registers.b() = 1;
	instruction.execute();
	CHECK(registers.c_flag());

	instruction.execute();
	CHECK_FALSE(registers.c_flag());
}

TEST_CASE("add_a_r8 applies half carry flag properly")
{
	cpu::registers registers{};
	cpu::add_a_r8 instruction{ registers.a(), registers.b(), registers.flags()};

	instruction.execute();
	CHECK_FALSE(registers.h_flag());

	registers.b() = 0xF;
	instruction.execute();
	CHECK_FALSE(registers.h_flag());

	instruction.execute();
	CHECK(registers.h_flag());

	registers.b() = 0;
	instruction.execute();
	CHECK_FALSE(registers.h_flag());
}
