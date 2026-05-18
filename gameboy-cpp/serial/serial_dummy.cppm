
export module serial:dummy;
import std;

namespace serial
{
    export struct dummy_link
    {
        [[nodiscard]] std::uint8_t transfer_bit(const std::uint8_t) { return 0x00; }
    };

}