#include "doctest.h"

import std;

namespace
{
    std::vector<std::uint8_t> read_rom(const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    }

}

TEST_CASE("acid.PPU generates output equals to reference image")
{
}
