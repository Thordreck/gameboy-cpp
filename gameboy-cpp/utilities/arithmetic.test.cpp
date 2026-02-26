
#include "doctest.h"

import utilities;
import std;

namespace 
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

TEST_CASE("arithmetic.8 bit add overflows are properly detected")
{
	CHECK_FALSE(utils::check_add_overflow(0xF_u8, 0xF_u8));
	CHECK_FALSE(utils::check_add_overflow(0xF_u8, 0_u8));
	CHECK(utils::check_add_overflow(0xFF_u8, 1_u8));
}

TEST_CASE("arithmetic.8 bit half add overflows are properly detected")
{
	CHECK_FALSE(utils::check_half_add_overflow(0x0_u8, 0x0_u8));
	CHECK(utils::check_half_add_overflow(0x0F_u8, 0x01_u8));
}

TEST_CASE("arithmetic.8 bit substract underflows are properly detected")
{
	CHECK_FALSE(utils::check_substract_underflow(0xF_u8, 0xF_u8));
	CHECK_FALSE(utils::check_substract_underflow(0xF_u8, 0xA_u8));
	CHECK(utils::check_substract_underflow(0x0A_u8, 0x0B_u8));
}

TEST_CASE("arithmetic.8 bit half substract underflows are properly detected")
{
	CHECK_FALSE(utils::check_substract_underflow(0xF_u8, 0xF_u8));
	CHECK_FALSE(utils::check_substract_underflow(0xF_u8, 0xA_u8));
	CHECK(utils::check_substract_underflow(0x0A_u8, 0x0B_u8));
}

TEST_CASE("arithmetic.16 bit add overflows are properly detected")
{
	CHECK_FALSE(utils::check_add_overflow(0xFF_u16, 0xFF_u16));
	CHECK_FALSE(utils::check_add_overflow(0xFF_u16, 0_u16));
	CHECK(utils::check_add_overflow(0xFFFF_u16, 1_u16));
}

TEST_CASE("arithmetic.8 bit half add overflows are properly detected")
{
	CHECK_FALSE(utils::check_half_add_overflow(0x0_u8, 0x0_u8));
	CHECK(utils::check_half_add_overflow(0x0F_u8, 0x01_u8));
}

TEST_CASE("arithmetic.16 bit substract underflows are properly detected")
{
	CHECK_FALSE(utils::check_substract_underflow(0xF_u16, 0xF_u16));
	CHECK_FALSE(utils::check_substract_underflow(0xF_u16, 0xA_u16));
	CHECK(utils::check_substract_underflow(0xFFFA_u8, 0xFFFB_u8));
}

TEST_CASE("arithmetic.16 bit half substract underflows are properly detected")
{
	CHECK_FALSE(utils::check_substract_underflow(0xFFFF_u16, 0xFFFF_u16));
	CHECK_FALSE(utils::check_substract_underflow(0xFFFB_u8, 0xFFFA_u8));
	CHECK(utils::check_substract_underflow(0xFFFA_u8, 0xFFFB_u8));
}
