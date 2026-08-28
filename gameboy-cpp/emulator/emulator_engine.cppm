module;
#include "profiling.hpp"

export module emulator.engine;

import :cpu;
import :memory;
import :scheduler;

import cpu;
import mbc;
import timer;
import audio;
import joypad;
import serial;
import graphics;
import cartridge;
import utilities;
import interrupts;
import emulator.core;

namespace emulator
{
    export class base_engine
    {
    public:
        virtual ~base_engine() = default;

        [[nodiscard]] virtual lcd_view_t lcd() const = 0;
        [[nodiscard]] virtual memory_view memory() const = 0;

        virtual void update_joypad_state(joypad::const_input_state_view_t state) = 0;
        virtual void tick(std::uint32_t num_ticks) = 0;
        virtual void tick_external_serial_clock() = 0;
    };

    export template<
        mbc::MemoryBankController MBC,
        graphics::FramebufferSink FramebufferSink,
        audio::AudioSink<float> AudioSink,
        serial::SerialInterface Serial>
    class engine : public base_engine
    {
    public:
        engine(MBC&& mbc_imp, FramebufferSink& framebuffer_sink, AudioSink& audio_sink, Serial& serial)
        : framebuffer_sink { framebuffer_sink }
        , serial { serial }
        , mbc { std::forward<MBC>(mbc_imp) }
        , memory_map { internal_memory, mbc, oam_dma, timers, interrupts, ppu_, joypad, serial_link, apu }
        , memory_buses { memory_map.get(), ppu_, oam_dma, apu }
        , cpu_runner { cpu }
        , audio_sink { audio_sink }
        {
            cpu.pc = 0x100;
            cpu.sp = 0xFFFE;
            cpu.reg.a() = 0x01;
            cpu.reg.f() = 0xB0;
            cpu.reg.b() = 0x00;
            cpu.reg.c() = 0x13;
            cpu.reg.d() = 0x00;
            cpu.reg.e() = 0xD8;
            cpu.reg.h() = 0x01;
            cpu.reg.l() = 0x4D;
            timers.set_divider(0xAC00);
        }

        ~engine() override = default;

        [[nodiscard]] lcd_view_t lcd() const override { return lcd_memory; }
        [[nodiscard]] memory_view memory() const override { return memory_view{ memory_map.get() }; }
        void update_joypad_state(const joypad::const_input_state_view_t state) override { joypad.set_state(state, interrupts); }

        void tick(const std::uint32_t num_ticks) override
        {
            PROFILER_SCOPE("Engine::tick()");

            batch_schedule(
                num_ticks,
                adapt_for_scheduler(cpu_runner, memory_buses.cpu_bus(), interrupts),
                adapt_for_scheduler(timers, interrupts),
                adapt_for_scheduler(ppu_, memory_buses.ppu_bus(), framebuffer_sink_adapter, interrupts),
                adapt_for_scheduler(oam_dma, memory_buses.oam_bus()),
                adapt_for_scheduler(apu, timers.divider(), internal_memory.wave_ram, audio_sink),
                adapt_for_scheduler(serial_link, serial, interrupts));
        }

        void tick_external_serial_clock() override
        {
            serial_link.external_tick(serial, interrupts);
        }

    private:
        cpu::cpu_state cpu {};
        timer::timer_system timers {};
        interrupts::interrupt_controller interrupts {};
        graphics::oam_dma oam_dma {};

        joypad::joypad joypad {};

        FramebufferSink& framebuffer_sink;
        graphics::lcd_memory_t lcd_memory {};
        graphics::lcd_framebuffer_sink_adapter<FramebufferSink> framebuffer_sink_adapter { framebuffer_sink, lcd_memory };
        graphics::pixel_processing_unit ppu_ {};

        Serial& serial;
        serial::link serial_link {};

        internal_memory internal_memory {};
        MBC mbc;
        memory_map<MBC> memory_map;
        memory_buses<MBC> memory_buses;

        cpu_runner cpu_runner;

        AudioSink& audio_sink;
        audio::audio_processing_unit apu {};
    };
}