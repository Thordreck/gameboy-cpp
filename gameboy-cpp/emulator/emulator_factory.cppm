
export module emulator.gameboy:factory;

import std;
import mbc;
import cartridge;
import emulator.engine;

namespace emulator
{
    export using base_engine_ptr = std::unique_ptr<base_engine>;

    export [[nodiscard]] std::expected<base_engine_ptr, std::string> create_engine(cartridge::rom& rom)
    {
        using namespace mbc;
        using enum cartridge::hardware_type;

        switch (rom.header.hardware)
        {
        case rom_only:
            if (rom.data.size() < rom_only_cartridge_data_size)
            {
                return std::unexpected { std::format("Unexpected no MBC rom size. Expected {}. Got {}", rom_only_cartridge_data_size, rom.data.size() ) };
            }

            return std::make_unique<engine<only_rom>>(only_rom { rom.data });
        case mbc1:
        case mbc1_ram:
        case mbc1_ram_battery:
            return std::make_unique<engine<mbc::mbc1>>(mbc::mbc1{ rom.data, rom.header.rom_size, rom.header.ram_size });
        case mbc3:
        case mbc3_ram:
        case mbc3_ram_battery:
        case mbc3_timer_battery:
        case mbc3_timer_ram_battery:
            return std::make_unique<engine<mbc::mbc3>>(mbc::mbc3{ rom.data, rom.header.rom_size, rom.header.ram_size });
        default:
            return std::unexpected{ std::format("Unsupported rom type {}", cartridge::pretty_print(rom.header.hardware)) };
        }
    }


}
