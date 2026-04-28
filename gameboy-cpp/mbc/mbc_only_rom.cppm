
export module mbc:only_rom;

import :common;
import std;

namespace mbc
{
    export constexpr std::size_t rom_only_data_size { 0x8000 };

    export class only_rom
    {
    public:
        only_rom(const const_rom_bank_t rom_bank_0, const const_rom_bank_t rom_bank_n)
            : rom_bank_0(rom_bank_0)
            , rom_bank_n(rom_bank_n)
        {}

        explicit only_rom(const const_rom_data_t rom)
            : rom_bank_0(rom.subspan<0x0000, 0x4000>())
            , rom_bank_n(rom.subspan<0x4000, 0x4000>())
        {}

        [[nodiscard]] std::uint8_t read_rom_bank_0(const std::uint16_t address) const
        {
            return rom_bank_0[address - rom_bank_0_start_address];
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            return rom_bank_n[address - rom_bank_n_start_address];
        }

        [[nodiscard]] std::uint8_t read_external_ram(const std::uint16_t) const
        {
            return 0xFF;
        }

        void write_rom_bank_0(const std::uint16_t, const std::uint8_t) {}
        void write_rom_bank_n(const std::uint16_t, const std::uint8_t) {}
        void write_external_ram(const std::uint16_t, const std::uint8_t) {}

    private:
        const_rom_bank_t rom_bank_0;
        const_rom_bank_t rom_bank_n;
    };

}