
#include "doctest.h"

import utilities;
import std;

TEST_CASE("littlen endian two-byte word is read and decoded properly")
{
	constexpr std::array<std::uint8_t, 4> data{ 0xAB, 0xCB, 0xDE, 0xFF };

	CHECK_EQ(utils::read_two_byte_little_endian(data, 0), 0XCBAB);
	CHECK_EQ(utils::read_two_byte_little_endian(data, 2), 0XFFDE);
}
