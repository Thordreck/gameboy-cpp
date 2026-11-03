
export module graphics:window;
import std;

namespace graphics
{
    export enum class window_tilemap_address_mode : std::uint16_t
    {
        _0x9800 = 0x9800,
        _0x9C00 = 0x9C00
    };

    export [[nodiscard]] std::uint16_t get_window_tilemap_area_address(const window_tilemap_address_mode mode)
    {
        return std::to_underlying(mode);
    }

}