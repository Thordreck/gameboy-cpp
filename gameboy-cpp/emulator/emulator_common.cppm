
export module emulator:common;

import std;
import audio;
import engine;
import memory;
import joypad;
import graphics;
import cartridge;

namespace emulator
{
    export using framebuffer_view_t = std::span<const std::uint8_t, graphics::lcd_memory_size>;
    export using framebuffer_t = std::array<std::uint8_t, graphics::lcd_memory_size>;
    export using volume_t = float;

    export template<typename T>
    concept JoypadSource = requires(T& source)
    {
        { source.read() } -> std::convertible_to<joypad::const_input_state_view_t>;
    };

    export template<typename T>
    concept AudioDevice = requires(T& device)
    {
        { device.open() } -> std::same_as<void>;
        { device.close() } -> std::same_as<void>;

        { device.resume() } -> std::same_as<void>;
        { device.suspend() } -> std::same_as<void>;
    };

    export template<typename T>
    concept Muteable = requires(T& device, const bool muted)
    {
        { device.set_muted(muted) } -> std::same_as<void>;
        { device.muted() } -> std::convertible_to<bool>;
    };

    export template<typename T>
    concept WithVolume = requires(T& device, const volume_t volume)
    {
        { device.set_volume(volume) } -> std::same_as<void>;
        { device.muted() } -> std::convertible_to<bool>;
    };

    export template<typename T, typename Sample>
    concept AudioOutputDevice = audio::AudioSink<T, Sample> && AudioDevice<T> && Muteable<T> && WithVolume<T>;

    export template<typename T>
    concept RendererTarget = requires(T& device)
    {
        { device.start_rendering_frames() } -> std::same_as<void>;
        { device.stop_rendering_frames() } -> std::same_as<void>;
    };

    export template<typename T>
    concept FramebufferRenderer = graphics::FramebufferSink<T> && RendererTarget<T>;

    export using load_rom_result_t = std::expected<void, std::string>;

    export template <typename T>
    concept Emulator = requires(
        T& emulator,
        const cartridge::rom& rom,
        const std::uint32_t ticks,
        const volume_t volume,
        const bool muted)
    {
        { emulator.load_rom(rom) } -> std::convertible_to<load_rom_result_t>;

        { emulator.cartridge() } -> std::convertible_to<cartridge::header>;
        { emulator.is_running() } -> std::convertible_to<bool>;
        { emulator.has_rom() } -> std::convertible_to<bool>;

        { emulator.resume() } -> std::same_as<void>;
        { emulator.pause() } -> std::same_as<void>;
        { emulator.stop() } -> std::same_as<void>;
        { emulator.step(ticks) } -> std::same_as<void>;

        { emulator.memory() } -> std::convertible_to<engine::memory_view>;

        { emulator.volume() } -> std::convertible_to<volume_t>;
        { emulator.set_volume(volume) } -> std::same_as<void>;

        { emulator.muted() } -> std::convertible_to<bool>;
        { emulator.set_muted(muted) } -> std::same_as<void>;
    };

}
