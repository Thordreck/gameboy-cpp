
export module mbc:common;
import std;

namespace mbc
{
    export constexpr std::size_t rom_bank_size = 0x4000;
    export constexpr std::size_t external_ram_size = 0x2000;

    export constexpr std::uint16_t rom_bank_0_start_address = 0x0000;
    export constexpr std::uint16_t rom_bank_n_start_address = 0x4000;
    export constexpr std::uint16_t external_ram_start_address = 0xA000;

    export using cartridge_data_t = std::span<std::uint8_t>;
    export using rom_bank_t = std::span<const std::uint8_t, rom_bank_size>;
    export using external_ram_t = std::array<std::uint8_t, external_ram_size>;

    export template<typename T>
    concept MemoryBankController = requires(T& mbc, const std::uint16_t address, const std::uint8_t value)
    {
        { mbc.read_rom_bank_0(address) } -> std::convertible_to<std::uint8_t>;
        { mbc.read_rom_bank_n(address) } -> std::convertible_to<std::uint8_t>;
        { mbc.read_external_ram(address) } -> std::convertible_to<std::uint8_t>;

        { mbc.write_rom_bank_0(address, value) } -> std::same_as<void>;
        { mbc.write_rom_bank_n(address, value) } -> std::same_as<void>;
        { mbc.write_external_ram(address, value) } -> std::same_as<void>;
    };

}