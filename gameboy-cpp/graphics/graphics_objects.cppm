
export module graphics:objects;
import std;

namespace graphics
{
    export enum class object_size : std::uint8_t
    {
        _8x8 = 8,
        _8x16 = 16
    };

    export [[nodiscard]] std::uint8_t get_objects_height(const object_size size)
    {
        return std::to_underlying(size);
    }

}