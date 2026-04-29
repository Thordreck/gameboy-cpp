
export module mbc:mbc1;

import std;
import :common;

namespace mbc
{
    export class mbc1
    {
        enum class banking_mode : std::uint8_t
        {
            simple,
            advanced
        };

    public:
        mbc1(const cartridge_data_t data, const std::uint64_t rom_size, const std::uint64_t ram_size)
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
            using enum banking_mode;
            const std::uint8_t raw_bank_index = bank_mode == simple ? 0 : bank_register_2 << 5;
            const std::uint8_t bank_index = raw_bank_index % rom_banks.size();

            return rom_banks[bank_index][address - rom_bank_0_start_address];
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            const std::uint8_t raw_bank_index = bank_register_2 << 5 | bank_register_1;
            const std::uint8_t bank_index = raw_bank_index % rom_banks.size();

            return rom_banks[bank_index][address - rom_bank_n_start_address];
        }

        [[nodiscard]] std::uint8_t read_external_ram(const std::uint16_t address) const
        {
            if (!ram_enabled || ram_banks.empty())
            {
                return 0xFF;
            }

            using enum banking_mode;
            const std::uint8_t raw_bank_index = bank_mode == simple ? 0 : bank_register_2;
            const std::uint8_t bank_index = raw_bank_index % ram_banks.size();

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
                using enum banking_mode;
                const std::uint8_t raw_bank_index = bank_mode == simple ? 0 : bank_register_2;
                const std::uint8_t bank_index = raw_bank_index % ram_banks.size();

                ram_banks[bank_index][address - external_ram_start_address] = value;
            }
        }

    private:
        void write_registers(const std::uint16_t address, const std::uint8_t value)
        {
            if (constexpr std::uint16_t ram_enable_end_address { 0x1FFF }; address <= ram_enable_end_address)
            {
                constexpr std::uint8_t enable_ram_value = 0x0A;
                constexpr std::uint8_t enable_ram_mask = 0x0F;

                ram_enabled = (value & enable_ram_mask) == enable_ram_value;
            }
            else if (constexpr std::uint16_t bank_register_1_end_address { 0x3FFF }; address <= bank_register_1_end_address)
            {
                constexpr std::uint8_t bank_register_1_write_mask = 0x1F;
                const std::uint8_t raw_bank_register_1 = bank_register_1_write_mask & value;

                bank_register_1 = std::max<std::uint8_t>(1, raw_bank_register_1);
            }
            else if (constexpr std::uint16_t bank_register_2_end_address { 0x5FFF }; address <= bank_register_2_end_address)
            {
                constexpr std::uint8_t bank_register_2_write_mask = 0b11;

                bank_register_2 = bank_register_2_write_mask & value;
            }
            else if (constexpr std::uint16_t bank_mode_select_end_address { 0x7FFF }; address <= bank_mode_select_end_address)
            {
                using enum banking_mode;
                constexpr std::uint8_t banking_mode_select_mask = 0x01;
                bank_mode = (value & banking_mode_select_mask) == 0 ? simple : advanced;
            }
        }

        std::vector<rom_bank_t> rom_banks{};
        std::vector<external_ram_t> ram_banks {};

        bool ram_enabled { false };
        std::uint8_t bank_register_1 { 1 };
        std::uint8_t bank_register_2 { 0 };
        banking_mode bank_mode { banking_mode::simple };
    };

}