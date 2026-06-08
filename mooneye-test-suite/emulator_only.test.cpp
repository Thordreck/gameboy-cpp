import mooneye;
#include "doctest.h"

// MBC1
TEST_CASE("mooneye.emulator_only.mbc1.bits_bank1")
{
    mooneye::run_test("roms/emulator-only/mbc1/bits_bank1.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.bits_bank2")
{
    mooneye::run_test("roms/emulator-only/mbc1/bits_bank2.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.bits_mode")
{
    mooneye::run_test("roms/emulator-only/mbc1/bits_mode.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.bits_ramg")
{
    mooneye::run_test("roms/emulator-only/mbc1/bits_ramg.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.multicart_rom_8Mb")
{
    mooneye::run_test("roms/emulator-only/mbc1/multicart_rom_8Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.ram_64kb")
{
    mooneye::run_test("roms/emulator-only/mbc1/ram_64kb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.ram_256kb")
{
    mooneye::run_test("roms/emulator-only/mbc1/ram_256kb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_1Mb")
{
    mooneye::run_test("roms/emulator-only/mbc1/rom_1Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_2Mb")
{
    mooneye::run_test("roms/emulator-only/mbc1/rom_2Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_4Mb")
{
    mooneye::run_test("roms/emulator-only/mbc1/rom_4Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_8Mb")
{
    mooneye::run_test("roms/emulator-only/mbc1/rom_8Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_16Mb")
{
    mooneye::run_test("roms/emulator-only/mbc1/rom_16Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc1.rom_512kb")
{
    mooneye::run_test("roms/emulator-only/mbc1/rom_512kb.gb");
}


// MBC2
TEST_CASE("mooneye.emulator_only.mbc2.bits_ramg")
{
    mooneye::run_test("roms/emulator-only/mbc2/bits_ramg.gb");
}

TEST_CASE("mooneye.emulator_only.mbc2.bits_romb")
{
    mooneye::run_test("roms/emulator-only/mbc2/bits_romb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc2.bits_unused")
{
    mooneye::run_test("roms/emulator-only/mbc2/bits_unused.gb");
}

TEST_CASE("mooneye.emulator_only.mbc2.ram")
{
    mooneye::run_test("roms/emulator-only/mbc2/ram.gb");
}

TEST_CASE("mooneye.emulator_only.mbc2.rom_1Mb")
{
    mooneye::run_test("roms/emulator-only/mbc2/rom_1Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc2.rom_2Mb")
{
    mooneye::run_test("roms/emulator-only/mbc2/rom_2Mb.gb");
}

TEST_CASE("mooneye.emulator_only.mbc2.rom_512kb")
{
    mooneye::run_test("roms/emulator-only/mbc2/rom_512kb.gb");
}
