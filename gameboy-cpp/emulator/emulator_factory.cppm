module;
#include "profiling.hpp"

export module emulator.gameboy:factory;

import std;
import mbc;
import audio;
import serial;
import graphics;
import cartridge;
import emulator.engine;

namespace emulator
{
    export using base_engine_ptr = std::unique_ptr<base_engine>;

    export template<graphics::FramebufferSink FramebufferSink, audio::AudioSink<float> AudioSink, serial::SerialInterface Serial>
    [[nodiscard]] std::expected<base_engine_ptr, std::string> create_engine(
        cartridge::rom& rom,
        FramebufferSink& framebuffer_sink,
        AudioSink& audio_sink,
        Serial& serial)
    {
        using namespace mbc;
        using enum cartridge::hardware_type;

        switch (rom.header.hardware)
        {
        case rom_only:
            if (rom.data.size() < rom_only_cartridge_data_size)
            {
                return std::unexpected { std::format("Unexpected no MBC rom size. Expected {}. Got {}", rom_only_cartridge_data_size, rom.data.size() ) };
            }

            return std::make_unique<engine<only_rom, FramebufferSink, AudioSink, Serial>>(only_rom { rom.data }, framebuffer_sink, audio_sink, serial);
        case mbc1:
        case mbc1_ram:
        case mbc1_ram_battery:
            return std::make_unique<engine<mbc::mbc1, FramebufferSink, AudioSink, Serial>>(mbc::mbc1{ rom.data, rom.header.rom_size, rom.header.ram_size }, framebuffer_sink, audio_sink, serial);
        case mbc2:
        case mbc2_battery:
            return std::make_unique<engine<mbc::mbc2, FramebufferSink, AudioSink, Serial>>(mbc::mbc2{ rom.data, rom.header.rom_size }, framebuffer_sink, audio_sink, serial);
        case mbc3:
        case mbc3_ram:
        case mbc3_ram_battery:
        case mbc3_timer_battery:
        case mbc3_timer_ram_battery:
            return std::make_unique<engine<mbc::mbc3, FramebufferSink, AudioSink, Serial>>(mbc::mbc3{ rom.data, rom.header.rom_size, rom.header.ram_size }, framebuffer_sink, audio_sink, serial);
        case mbc5:
        case mbc5_ram:
        case mbc5_ram_battery:
        case mbc5_rumble:
        case mbc5_rumble_ram:
        case mbc5_rumble_ram_battery:
            return std::make_unique<engine<mbc::mbc5, FramebufferSink, AudioSink, Serial>>(mbc::mbc5{ rom.data, rom.header.rom_size, rom.header.ram_size }, framebuffer_sink, audio_sink, serial);
        default:
            return std::unexpected{ std::format("Unsupported rom type {}", cartridge::pretty_print(rom.header.hardware)) };
        }
    }


}
