
export module cartridge:rom;

import std;
import :header;

namespace cartridge
{
    export struct rom
    {
        header header;
        std::vector<std::uint8_t> data;
    };

}