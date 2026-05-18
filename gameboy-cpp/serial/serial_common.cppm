
export module serial:common;

import std;

namespace serial
{

    export template <typename Imp>
    concept SerialInterface = requires(Imp& serial, const std::uint8_t bit)
    {
        { serial.transfer_bit(bit) } -> std::convertible_to<std::uint8_t>;
    };

}