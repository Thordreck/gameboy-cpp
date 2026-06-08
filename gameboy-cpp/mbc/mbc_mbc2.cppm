export module mbc:mbc2;

import std;
import :common;

namespace mbc
{
    export class mbc2
    {
    public:
        mbc2(const cartridge_data_t data, const std::uint64_t rom_size)
        {
            // ROM
            const std::size_t rom_bank_count = rom_size / rom_bank_size;
            rom_banks.reserve(rom_bank_count);

            for (std::size_t i = 0; i < rom_bank_count; ++i)
            {
                rom_banks.emplace_back(data.subspan(i * rom_bank_size, rom_bank_size));
            }
        }

        [[nodiscard]] std::uint8_t read_rom_bank_0(const std::uint16_t address) const
        {
            return rom_banks[0][address - rom_bank_0_start_address];
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            const std::uint8_t effective_rom_bank_index = rom_bank_index % rom_banks.size();
            return rom_banks[effective_rom_bank_index][address - rom_bank_n_start_address];
        }

        [[nodiscard]] std::uint8_t read_external_ram(const std::uint16_t address) const
        {
            if (!ram_enabled)
            {
                return 0xFF;
            }

            const std::uint16_t ram_index = address & 0x1FF;
            return built_in_ram[ram_index] | 0xF0;
        }

        void write_rom_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
            const bool register_flag = address & 0x100;

            if (!register_flag)
            {
                ram_enabled = (value & 0x0F) == 0x0A;
            }
            else
            {
                rom_bank_index = std::max(1, value & 0xF);
            }
        }

        void write_rom_bank_n(const std::uint16_t, const std::uint8_t)
        {}

        void write_external_ram(const std::uint16_t address, const std::uint8_t value)
        {
            if (ram_enabled)
            {
                const std::uint16_t ram_index = address & 0x1FF;
                built_in_ram[ram_index] = value & 0x0F;
            }
        }

    private:
        std::vector<rom_bank_t> rom_banks{};
        std::array<std::uint8_t, 512> built_in_ram {};

        bool ram_enabled { false };
        std::uint8_t rom_bank_index { 1 };
    };

}