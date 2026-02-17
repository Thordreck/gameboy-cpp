
#include "doctest.h"

import cpu;
import std;

namespace
{
	std::vector<std::uint8_t> read_rom(std::filesystem::path filepath)
	{
		std::ifstream file(filepath, std::ios::binary);
		return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	}
}

TEST_CASE("blargg.06-ld r,r")
{
	const std::filesystem::path rom_file{ "06-ld r,r.gb" };

	REQUIRE(std::filesystem::exists(rom_file));
	const std::vector<std::uint8_t> rom_data = read_rom(rom_file);

	//cpu::cpu cpu{};
	//std::copy(rom_data.cbegin(), rom_data.cend(), cpu.memory.begin());
}
