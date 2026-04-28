
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
            if (rom.data.size() < rom_only_data_size)
            {
                return std::unexpected { std::format("Unexpected no MBC rom size. Expected {}. Got {}", rom_only_data_size, rom.data.size() ) };
            }

            return std::make_unique<engine<only_rom>>(only_rom { rom.data });
        default:
            return std::unexpected{ std::format("Unsupported rom type {}", cartridge::pretty_print(rom.header.hardware)) };
        }
    }


}
