import mooneye;
#include "doctest.h"

TEST_CASE("mooneye.emulator_only.mbc1.bits_bank1")
{
    mooneye::run_test("bits_bank1.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.bits_bank2")
{
    mooneye::run_test("bits_bank2.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.bits_mode")
{
    mooneye::run_test("bits_mode.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.bits_ramg")
{
    mooneye::run_test("bits_ramg.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.multicart_rom_8Mb")
{
    mooneye::run_test("multicart_rom_8Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.ram_64kb")
{
    mooneye::run_test("ram_64kb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.ram_256kb")
{
    mooneye::run_test("ram_256kb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_1Mb")
{
    mooneye::run_test("rom_1Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_2Mb")
{
    mooneye::run_test("rom_2Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_4Mb")
{
    mooneye::run_test("rom_4Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_8Mb")
{
    mooneye::run_test("rom_8Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_16Mb")
{
    mooneye::run_test("rom_16Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_512kb")
{
    mooneye::run_test("rom_512kb.gb");
}
