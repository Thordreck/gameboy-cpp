
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

    export constexpr std::uint8_t num_joypad_inputs { 8 };
    export using const_input_state_view_t = std::span<const bool, num_joypad_inputs>;

    export template <typename T>
    concept JoypadReadableInput = requires(const T& provider)
    {
        { provider.get_state() } -> std::convertible_to<const_input_state_view_t>;
    };

    export template <typename T>
    concept JoypadWritableInput = requires(T& provider, const_input_state_view_t state)
    {
        { provider.set_state(state) } -> std::same_as<void>;
    };

    export template <typename T>
    concept JoypadInput = JoypadReadableInput<T> && JoypadWritableInput<T>;

    export template <typename T>
    concept JoypadReadableSource = requires(const T& provider)
    {
        { provider.get_source() } -> std::convertible_to<joypad_source>;
    };

    export template <typename T>
    concept JoypadWritableSource = requires(T& provider, const joypad_source source)
    {
        { provider.set_source(source) } -> std::same_as<void>;
    };

    export template <typename T>
    concept JoypadSource = JoypadReadableSource<T> && JoypadWritableSource<T>;

    export template <typename T>
    concept MultiplexedReadableJoypad = JoypadReadableInput<T> && JoypadReadableSource<T>;

    export template <typename T>
    concept MultiplexedWritableJoypad = JoypadWritableSource<T> && JoypadWritableInput<T>;

    export template <typename T>
    concept MultiplexedJoypad = JoypadInput<T> && JoypadSource<T>;

}