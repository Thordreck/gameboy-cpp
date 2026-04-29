
export module mbc:mbc3;

import std;
import :common;

namespace mbc
{
    export class mbc3
    {
    public:
        mbc3(const cartridge_data_t data, const std::uint64_t rom_size, const std::uint64_t ram_size)
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
            const std::uint8_t bank_index = rom_bank_register % rom_banks.size();
            return rom_banks[bank_index][address - rom_bank_n_start_address];
        }

        [[nodiscard]] std::uint8_t read_external_ram(const std::uint16_t address) const
        {
            // TODO: timer support
            if (!ram_timer_enabled || ram_timer_register > 0x07 || ram_banks.empty())
            {
                return 0xFF;
            }

            const std::uint8_t bank_index = ram_timer_register % ram_banks.size();
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
            // TODO: timer support
            if (ram_timer_enabled && ram_timer_register <= 0x07 && !ram_banks.empty())
            {
                const std::uint8_t bank_index = ram_timer_register % ram_banks.size();
                ram_banks[bank_index][address - external_ram_start_address] = value;
            }
        }

    private:
        void write_registers(const std::uint16_t address, const std::uint8_t value)
        {
            if (constexpr std::uint16_t ram_timer_enable_end_address { 0x1FFF }; address <= ram_timer_enable_end_address)
            {
                constexpr std::uint8_t enable_ram_timer_value = 0x0A;
                constexpr std::uint8_t enable_ram_timer_mask = 0x0F;

                ram_timer_enabled = (value & enable_ram_timer_mask) == enable_ram_timer_value;
            }
            else if (constexpr std::uint16_t rom_bank_register_end_address { 0x3FFF }; address <= rom_bank_register_end_address)
            {
                constexpr std::uint8_t bank_register_1_write_mask = 0x7F;
                const std::uint8_t raw_bank_register_1 = bank_register_1_write_mask & value;

                rom_bank_register = std::max<std::uint8_t>(1, raw_bank_register_1);
            }
            else if (constexpr std::uint16_t ram_timer_register_end_address { 0x5FFF }; address <= ram_timer_register_end_address)
            {
                ram_timer_register = value;
            }
            else if (constexpr std::uint16_t latch_clock_data_end_address { 0x7FFF }; address <= latch_clock_data_end_address)
            {
                // TODO: implement RTC
            }
        }

        std::vector<rom_bank_t> rom_banks{};
        std::vector<external_ram_t> ram_banks {};

        bool ram_timer_enabled { false };
        std::uint8_t rom_bank_register { 1 };
        std::uint8_t ram_timer_register { 0 };
    };

}
