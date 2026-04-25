module;
#include "profiling.hpp"

export module emulator.engine;

import :cpu;
import :memory;
import :scheduler;

import cpu;
import mbc;
import timer;
import joypad;
import graphics;
import cartridge;
import interrupts;
import emulator.core;

namespace emulator
{
    using lcd_memory_t = std::array<std::uint8_t, graphics::lcd_memory_size>;

    export class engine
    {
    public:
        explicit engine(const cartridge::rom& rom)
        : lcd_adapter { lcd_memory }
        , memory_map { internal_memory, oam_dma, timers, interrupts, ppu_, joypad }
        , memory_buses { memory_map.get(), ppu_, oam_dma }
        , cpu_runner { cpu }
        , cartridge_header { rom.header }
        {
            // TODO: proper mbc support
            std::ranges::copy(rom.data | std::views::take(internal_memory.rom_bank_0.size()), internal_memory.rom_bank_0.begin());
            std::ranges::copy(rom.data | std::views::drop(internal_memory.rom_bank_0.size()), internal_memory.rom_bank_n.begin());

            cpu.pc = 0x100;
            cpu.sp = 0xFFFE;
            timers.divider() = 0xAB;
        }

        [[nodiscard]] lcd_view_t lcd() const { return lcd_memory; }
        [[nodiscard]] memory_map_t& memory() { return memory_map.get(); }
        [[nodiscard]] cartridge::header cartridge() const { return cartridge_header; }

        void update_joypad_state(const joypad::const_input_state_view_t state) { joypad.set_state(state); }

        void tick(const std::uint32_t num_ticks)
        {
            PROFILER_SCOPE("Engine::tick()");

            batch_schedule(
                num_ticks,
                adapt_for_scheduler(cpu_runner, memory_buses.cpu_bus()),
                adapt_for_scheduler(timers, memory_buses.timers_bus()),
                adapt_for_scheduler(ppu_, memory_buses.ppu_bus(), lcd_adapter),
                adapt_for_scheduler(oam_dma, memory_buses.oam_bus()));
        }

    private:
        cpu::cpu_state cpu {};
        timer::timer_system timers {};
        interrupts::interrupt_registers interrupts {};
        graphics::oam_dma oam_dma {};

        joypad::joypad joypad {};

        lcd_memory_t lcd_memory {};
        graphics::raw_memory_lcd lcd_adapter;
        graphics::pixel_processing_unit ppu_ {};

        internal_memory internal_memory {};
        mbc::external_memory external_memory {};
        memory_map memory_map;
        memory_buses memory_buses;

        cpu_runner cpu_runner;
        cartridge::header cartridge_header;
    };

}