
export module mbc:common;
import std;

namespace mbc
{
    export using rom_bank_t = std::array<std::uint8_t, 0x4000>;
    export using ram_bank_t = std::array<std::uint8_t, 0x1000>;

    export struct external_memory
    {
        rom_bank_t rom_bank_0;
        std::vector<rom_bank_t> rom_bank_n;
        ram_bank_t ram_bank_0;
    };

    export template<typename T>
    concept MBCImplementation = requires(const T& mbc, const std::uint16_t address, const std::uint8_t value)
    {
        { mbc.read_rom_bank_0(address) } -> std::convertible_to<std::uint8_t>;
        { mbc.read_rom_bank_n(address) } -> std::convertible_to<std::uint8_t>;
        { mbc.read_ram_bank_0(address) } -> std::convertible_to<std::uint8_t>;

        { mbc.write_rom_bank_0(address, value) } -> std::same_as<void>;
        { mbc.write_rom_bank_n(address, value) } -> std::same_as<void>;
        { mbc.write_ram_bank_0(address, value) } -> std::same_as<void>;
    };

}