export module mbc:system;

import :only_rom;
import :mbc1;
import std;

namespace mbc
{
    using mbcs_variant_t = std::variant<std::monostate, only_rom>;

    // TODO: rethink this approach. This causes a dispatch every time the external memory is accessed.
    // It might be better to simply use a function pointer
    export class mbc_system
    {
    public:
        template<MBCImplementation MBC>
        void load(MBC&& mbc)
        {
            controller = std::forward<MBC>(mbc);
        }

        [[nodiscard]] std::uint8_t read_rom_bank_0(const std::uint16_t address) const
        {
            return 0xFF;
            //return std::visit([address](const auto& mbc) { return mbc.read_rom_0(address); }, controller);
        }

        [[nodiscard]] std::uint8_t read_rom_bank_n(const std::uint16_t address) const
        {
            return 0xFF;
            //return std::visit([address](const auto& mbc) { return mbc.read_rom_n(address); }, controller);
        }

        [[nodiscard]] std::uint8_t read_ram_bank_0(const std::uint16_t address) const
        {
            return 0xFF;
            //return std::visit([address](const auto& mbc) { return mbc.read_ram(address); }, controller);
        }

        void write_rom_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
            //std::visit([address, value](auto& mbc) { mbc.write_rom_0(address, value); }, controller);
        }

        void write_rom_bank_n(const std::uint16_t address, const std::uint8_t value)
        {
            //std::visit([address, value](auto& mbc) { mbc.write_rom_n(address, value); }, controller);
        }

        void write_ram_bank_0(const std::uint16_t address, const std::uint8_t value)
        {
            //std::visit([address, value](auto& mbc) { mbc.write_rom_n(address, value); }, controller);
        }

    private:
        mbcs_variant_t controller {};
    };
}