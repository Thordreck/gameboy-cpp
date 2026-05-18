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
import serial;
import graphics;
import cartridge;
import interrupts;
import emulator.core;

namespace emulator
{
    using lcd_memory_t = std::array<std::uint8_t, graphics::lcd_memory_size>;

    export class base_engine
    {
    public:
        virtual ~base_engine() = default;
        [[nodiscard]] virtual lcd_view_t lcd() const = 0;
        virtual void update_joypad_state(joypad::const_input_state_view_t state) = 0;
        virtual void tick(std::uint32_t num_ticks) = 0;
        virtual void tick_external_serial_clock() = 0;
    };

    export template<mbc::MemoryBankController MBC, serial::SerialInterface Serial>
    class engine : public base_engine
    {
    public:
        engine(MBC&& mbc_imp, Serial& serial)
        : lcd_adapter { lcd_memory }
        , mbc { std::forward<MBC>(mbc_imp) }
        , serial { serial }
        , memory_map { internal_memory, mbc, oam_dma, timers, interrupts, ppu_, joypad, serial_link }
        , memory_buses { memory_map.get(), ppu_, oam_dma }
        , cpu_runner { cpu }
        {
            cpu.pc = 0x100;
            cpu.sp = 0xFFFE;
            timers.divider() = 0xAB;
        }

        ~engine() override = default;

        [[nodiscard]] lcd_view_t lcd() const override { return lcd_memory; }
        [[nodiscard]] auto& memory() { return memory_map.get(); }

        void update_joypad_state(const joypad::const_input_state_view_t state) override { joypad.set_state(state); }

        void tick(const std::uint32_t num_ticks) override
        {
            PROFILER_SCOPE("Engine::tick()");

            batch_schedule(
                num_ticks,
                adapt_for_scheduler(cpu_runner, memory_buses.cpu_bus()),
                adapt_for_scheduler(timers, memory_buses.timers_bus()),
                adapt_for_scheduler(ppu_, memory_buses.ppu_bus(), lcd_adapter),
                adapt_for_scheduler(oam_dma, memory_buses.oam_bus()),
                adapt_for_scheduler(serial_link, serial));
        }

        void tick_external_serial_clock() override
        {
            serial_link.external_tick(serial);
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

        Serial& serial;
        serial::link serial_link {};

        internal_memory internal_memory {};
        MBC mbc;
        memory_map<MBC> memory_map;
        memory_buses<MBC> memory_buses;

        cpu_runner cpu_runner;
    };
}