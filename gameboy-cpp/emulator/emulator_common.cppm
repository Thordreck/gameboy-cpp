
export module emulator.core:common;

import std;
import memory;
import joypad;
import graphics;
import cartridge;

namespace emulator
{
    export using framebuffer_view_t = std::span<const std::uint8_t, graphics::lcd_memory_size>;
    export using load_rom_result_t = std::expected<void, std::string>;

    export template <typename T>
    concept Emulator = requires(T& emulator, const cartridge::rom& rom)
    {
        { emulator.load_rom(rom) } -> std::convertible_to<load_rom_result_t>;

        { emulator.cartridge() } -> std::convertible_to<cartridge::header>;
        { emulator.framebuffer() } -> std::convertible_to<framebuffer_view_t>;
        { emulator.is_running() } -> std::convertible_to<bool>;
        { emulator.has_rom() } -> std::convertible_to<bool>;

        { emulator.resume() } -> std::same_as<void>;
        { emulator.pause() } -> std::same_as<void>;
        { emulator.stop() } -> std::same_as<void>;
    };

    export using lcd_view_t = std::span<const memory::memory_data_t, graphics::lcd_memory_size>;

    export template <typename T>
    concept Engine = requires(T& engine, const std::uint32_t num_ticks, const joypad::const_input_state_view_t joypad_state)
    {
        { engine.tick(num_ticks) } -> std::same_as<void>;
        { engine.lcd() } -> std::convertible_to<lcd_view_t>;
        { engine.update_joypad_state(joypad_state) } -> std::same_as<void>;
    };

    export template<typename T>
    concept JoypadSource = requires(T& source)
    {
        { source.read() } -> std::convertible_to<joypad::const_input_state_view_t>;
    };

}
