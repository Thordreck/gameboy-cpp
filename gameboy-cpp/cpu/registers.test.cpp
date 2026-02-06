#include "doctest.h"

import cpu;
import std;

namespace cpu 
{
	static doctest::String toString(const register_8& reg)
	{
		return { std::format("{}", static_cast<uint8_t>(reg)).c_str() };
	}

	static doctest::String toString(const register_16& reg)
	{
		return { std::format("{}", static_cast<uint16_t>(reg)).c_str() };
	}
}

TEST_CASE("8-bit registers are zero-initialized")
{
	cpu::registers registers{};

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("16-bit registers are zero-initialized")
{
	cpu::registers registers{};

	CHECK(registers.af() == 0);
	CHECK(registers.bc() == 0);
	CHECK(registers.de() == 0);
	CHECK(registers.hl() == 0);
}

TEST_CASE("a register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.a() = test_value;

	CHECK(registers.a() == test_value);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("b register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.b() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == test_value);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("c register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.c() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == test_value);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("d register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.d() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == test_value);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("e register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.e() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == test_value);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("f register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.f() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == test_value);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("h register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.h() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == test_value);
	CHECK(registers.l() == 0);
}

TEST_CASE("l register updates its value properly")
{
	cpu::registers registers{};

	const std::uint8_t test_value = 0xAF;
	registers.l() = test_value;

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == test_value);
}

TEST_CASE("af register updates its value properly")
{
	cpu::registers registers{};

	const std::uint16_t test_value = 0xABCD;
	registers.af() = test_value;

	CHECK_EQ(registers.af(), test_value);
	CHECK_EQ(registers.a(), 0xAB);
	CHECK_EQ(registers.f(), 0xCD);

	CHECK_EQ(registers.b(), 0);
	CHECK_EQ(registers.c(), 0);
	CHECK_EQ(registers.d(), 0);
	CHECK_EQ(registers.e(), 0);
	CHECK_EQ(registers.h(), 0);
	CHECK_EQ(registers.l(), 0);
}

TEST_CASE("bc register updates its value properly")
{
	cpu::registers registers{};

	const std::uint16_t test_value = 0xABCD;
	registers.bc() = test_value;

	CHECK(registers.bc() == test_value);
	CHECK(registers.b() == 0xAB);
	CHECK(registers.c() == 0xCD);

	CHECK(registers.a() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("de register updates its value properly")
{
	cpu::registers registers{};

	const std::uint16_t test_value = 0xABCD;
	registers.de() = test_value;

	CHECK(registers.de() == test_value);
	CHECK(registers.d() == 0xAB);
	CHECK(registers.e() == 0xCD);

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.f() == 0);
	CHECK(registers.h() == 0);
	CHECK(registers.l() == 0);
}

TEST_CASE("hl register updates its value properly")
{
	cpu::registers registers{};

	const std::uint16_t test_value = 0xABCD;
	registers.hl() = test_value;

	CHECK(registers.hl() == test_value);
	CHECK(registers.h() == 0xAB);
	CHECK(registers.l() == 0xCD);

	CHECK(registers.a() == 0);
	CHECK(registers.b() == 0);
	CHECK(registers.c() == 0);
	CHECK(registers.d() == 0);
	CHECK(registers.e() == 0);
	CHECK(registers.f() == 0);
}
