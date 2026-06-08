export module mbc:mbc5;

import std;
import :common;

namespace mbc
{
    export class mbc5
    {
    public:
        mbc5(const cartridge_data_t data, const std::uint64_t rom_size, const std::uint64_t ram_size)
        {
            // ROM
            const std::size_t rom_bank_count = rom_size / rom_bank_size;
            rom_banks.reserve(rom_bank_count);

            for (std::size_t i = 0; i < rom_bank_count; ++i)
            {
                rom_banks.emplace_back(data.subspan(i * rom_bank_size, rom_bank_size));
            }

            // RAM
            const std::size_t ram_bank_count = ram_size / external_ram_size;
            ram_banks.resize(ram_bank_count);
        }

        [[nodiscard]] std::uint8_t read_rom_bank_0(const std::uint16_t address) const
        {
            return rom_banks[0][address - rom_bank_0_start_address];
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            const std::uint16_t bank_index = rom_bank_index % rom_banks.size();
            return rom_banks[bank_index][address - rom_bank_n_start_address];
        }

        [[nodiscard]] std::uint8_t read_external_ram(const std::uint16_t address) const
        {
            if (!ram_enabled || ram_banks.empty())
            {
                return 0xFF;
            }

            const std::uint8_t bank_index = ram_bank_index % ram_banks.size();
            return ram_banks[bank_index][address - external_ram_start_address];
        }

        void write_rom_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
            write_registers(address, value);
        }

        void write_rom_bank_n(const std::uint16_t address, const std::uint8_t value)
        {
            write_registers(address, value);
        }

        void write_external_ram(const std::uint16_t address, const std::uint8_t value)
        {
            if (ram_enabled && !ram_banks.empty())
            {
                const std::uint8_t bank_index = ram_bank_index % ram_banks.size();
                ram_banks[bank_index][address - external_ram_start_address] = value;
            }
        }

    private:
        void write_registers(const std::uint16_t address, const std::uint8_t value)
        {
            if (constexpr std::uint16_t ram_enable_end_address { 0x1FFF }; address <= ram_enable_end_address)
            {
                ram_enabled = (value & 0xF) == 0xA;
            }
            else if (constexpr std::uint16_t low_byte_rom_bank_address { 0x2FFF }; address <= low_byte_rom_bank_address)
            {
                rom_bank_index = (rom_bank_index & 0xFF00) | value;
            }
            else if (constexpr std::uint16_t last_byte_rom_bank_address { 0x3FFF }; address <= last_byte_rom_bank_address)
            {
                rom_bank_index = (rom_bank_index & 0xFF) | ((value & 0b1) << 8);
            }
            else if (constexpr std::uint16_t ram_bank_index_address { 0x5FFF }; address <= ram_bank_index_address)
            {
                ram_bank_index = value & 0xF;
            }
        }

        std::vector<rom_bank_t> rom_banks{};
        std::vector<external_ram_t> ram_banks {};

        bool ram_enabled { false };
        std::uint16_t rom_bank_index { 1 };
        std::uint8_t ram_bank_index { 0 };
    };

}
