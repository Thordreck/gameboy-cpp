
export module graphics:background;
import std;

namespace graphics
{
    export enum class background_tile_data_area : std::uint16_t
    {
        _0x8000 = 0x8000,
        _0x8800 = 0x9000,
    };

    export enum class background_tilemap_area : std::uint16_t
    {
        _0x9800 = 0x9800,
        _0x9C00 = 0x9C00
    };

    export [[nodiscard]] std::uint16_t get_background_tilemap_area_address(const background_tilemap_area area)
    {
        return std::to_underlying(area);
    }

    export [[nodiscard]] std::uint16_t get_background_tile_data_area_address(const background_tile_data_area area)
    {
        return std::to_underlying(area);
    }

}