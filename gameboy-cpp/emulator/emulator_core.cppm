module;
#include "profiling.hpp"

export module emulator.core;
export import :memory;
export import :cpu;
export import :joypad;
export import :scheduler;

import cpu;
import timer;
import joypad;
import graphics;
import interrupts;

namespace emulator
{
    export using lcd_memory_t = std::array<memory::memory_data_t, graphics::lcd_memory_size>;;
    export using lcd_view_t = std::span<const memory::memory_data_t, graphics::lcd_memory_size>;;
    export using rom_data_view_t = std::span<const std::uint8_t>;

    export class engine
    {
    public:
        engine()
            : lcd_adapter { lcd_memory }
            , lcd_ { lcd_adapter }
            , ppu_ { lcd_ }
            , memory_map { memory_blocks, oam_dma, timers, interrupts, ppu_, joypad_imp }
            , memory_buses { memory_map.get(), cpu, timers, ppu_, oam_dma, joypad_imp }
            , cpu_runner { cpu }
        {
            cpu.pc = 0x100;
            cpu.sp = 0xFFFE;
            timers.divider() = 0xAB;
        }

        ~engine() { stop(); }

        [[nodiscard]] lcd_view_t lcd() const { return lcd_memory; }
        [[nodiscard]] joypad_input_sink joypad() { return joypad_input_sink{ joypad_imp }; }
        [[nodiscard]] memory::memory_bus memory_bus() { return memory::memory_bus{ memory_map.get() }; }

        void tick(const std::uint32_t num_ticks)
        {
            PROFILER_SCOPE("Engine::tick()");

            // TODO: rethink this
            if (rom_loaded) [[likely]]
            {
                batch_schedule(num_ticks, cpu_runner, timers, ppu_, oam_dma);
            }
        }

        void reset()
        {
            // TODO: rethink this method
            cpu = {};
            timers = {};
            interrupts = {};
            oam_dma = {};
            lcd_memory.fill({});

            cpu.pc = 0x100;
            cpu.sp = 0xFFFE;
            timers.divider() = 0xAB;
        }

        void load_rom(const rom_data_view_t rom_data)
        {
            std::ranges::copy(rom_data | std::views::take(memory_blocks.rom_bank_0.size()), memory_blocks.rom_bank_0.begin());
            std::ranges::copy(rom_data | std::views::drop(memory_blocks.rom_bank_0.size()), memory_blocks.rom_bank_n.begin());

            rom_loaded = true;
        }

        void pause()
        {
        }

        void stop()
        {
        }

    private:
        cpu::cpu_state cpu {};
        timer::timer_system timers {};
        interrupts::interrupt_registers interrupts {};
        graphics::oam_dma oam_dma {};

        joypad::joypad joypad_imp {};

        lcd_memory_t lcd_memory {};
        graphics::raw_memory_lcd lcd_adapter;
        graphics::lcd lcd_;
        graphics::pixel_processing_unit ppu_;

        memory_blocks memory_blocks {};
        memory_map memory_map;
        memory_buses memory_buses;

        cpu_runner cpu_runner;

        // TODO: rethink this
        bool rom_loaded { false };
    };

}