
export module joypad:common;
import std;

namespace joypad
{
    export enum class joypad_source : std::uint8_t
    {
        none,
        buttons,
        d_pad,
        both,
    };

    export enum class joypad_input : std::uint8_t
    {
        start,
        select,
        up,
        down,
        left,
        right,
        b,
        a,
    };

    export template <typename T>
    concept JoypadInputProvider = requires(const T& provider, const joypad_input input)
    {
        { provider.is_input_pressed(input) } -> std::convertible_to<bool>;
    };

    export template <typename T>
    concept JoypadSourceProvider = requires(T& provider, const T& const_provider, const joypad_source source)
    {
        { provider.set_source(source) } -> std::same_as<void>;
        { const_provider.get_source() } -> std::convertible_to<joypad_source>;
    };

    export template <typename T>
    concept MultiplexedJoypadProvider = JoypadInputProvider<T> && JoypadSourceProvider<T>;

}