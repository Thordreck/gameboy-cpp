
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
            external_ram_banks.resize(ram_bank_count);
        }

        [[nodiscard]] std::uint8_t read_rom_bank_0(const std::uint16_t address) const
        {
            return rom_banks[0][address - rom_bank_0_start_address];
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            return rom_banks[rom_bank_index][address - rom_bank_n_start_address];
        }

        [[nodiscard]] std::uint8_t read_external_ram(const std::uint16_t address) const
        {
            return ram_enabled
                ? external_ram_banks[external_ram_bank_index][address - external_ram_start_address]
                : 0xFF;
        }

        void write_rom_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
            if (constexpr std::uint16_t ram_enable_end_address { 0x1FFF }; address <= ram_enable_end_address)
            {
                constexpr std::uint8_t enable_ram_value = 0x0A;
                constexpr std::uint8_t enable_ram_mask = 0x0F;

                ram_enabled = (value & enable_ram_mask) == enable_ram_value;
            }
            else if (constexpr std::uint16_t rom_bank_select_end_address { 0x3FFF }; address <= rom_bank_select_end_address)
            {
                constexpr std::uint8_t rom_bank_select_mask = 0x1F;
                const std::uint8_t raw_rom_bank_index = rom_bank_select_mask & value;
                const std::uint8_t rom_bank_index_clamp_mask = 0xFF >> (8 - rom_banks.size());
                const std::uint8_t clamped_rom_bank_index = raw_rom_bank_index & rom_bank_index_clamp_mask;

                rom_bank_index = std::max<std::uint8_t>(1, clamped_rom_bank_index);
            }
            else if (constexpr std::uint16_t ram_bank_select_end_address { 0x5FFF }; address <= rom_bank_select_end_address)
            {
                // TODO: implement
            }
            else
            {
                using enum banking_mode;
                constexpr std::uint8_t banking_mode_select_mask = 0x01;
                bank_mode = (value & banking_mode_select_mask) == 0 ? simple : advanced;
            }
        }

        void write_rom_bank_n(const std::uint16_t, const std::uint8_t) {}

        void write_external_ram(const std::uint16_t address, const std::uint8_t value)
        {
            if (ram_enabled)
            {
                external_ram_banks[external_ram_bank_index][address - external_ram_start_address] = value;
            }
        }

    private:
        std::vector<rom_bank_t> rom_banks{};
        std::vector<external_ram_t> external_ram_banks {};

        bool ram_enabled { false };
        std::uint8_t rom_bank_index { 1 };
        std::uint8_t external_ram_bank_index { 0 };
        banking_mode bank_mode { banking_mode::simple };

    };
}