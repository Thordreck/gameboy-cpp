
export module mbc:only_rom;

import :common;
import std;

namespace mbc
{
    export class only_rom
    {
    public:
        explicit only_rom(external_memory& memory) : memory(memory) {}

        [[nodiscard]] std::uint8_t read_rom_bank_0(const std::uint16_t address) const
        {
            return memory.rom_bank_0[address];
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            return memory.rom_bank_n[0][address - 0x4000];
        }

        [[nodiscard]] std::uint8_t read_ram_bank_0(const std::uint16_t address) const
        {
            return 0xFF;
        }

        void write_rom_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
            memory.rom_bank_0[address] = value;
        }

        void write_rom_bank_n(const std::uint16_t address, const std::uint8_t value)
        {
            memory.rom_bank_n[0][address - 0x4000] = value;
        }

        void write_ram_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
        }

    private:
        external_memory& memory;
    };

}