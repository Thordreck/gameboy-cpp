export module emulator.core:memory;

export import std;
export import timer;
export import memory;
export import joypad;
export import graphics;
export import utilities;
export import interrupts;

namespace emulator
{
    export using rom_bank_t = std::array<memory::memory_data_t, 0x4000>;
    export using vram_t = std::array<memory::memory_data_t, graphics::vram_size>;
    export using external_ram_t = std::array<memory::memory_data_t, 0x2000>;
    export using work_ram_t = std::array<memory::memory_data_t, 0x1000>;
    export using echo_ram_t = std::array<memory::memory_data_t, 0x1E00>;
    export using oam_memory_t = std::array<memory::memory_data_t, graphics::oam_size>;
    export using hram_t = std::array<memory::memory_data_t, 0x7F>;

    export struct memory_blocks
    {
        vram_t vram{};
        rom_bank_t rom_bank_0{};
        rom_bank_t rom_bank_n{};
        external_ram_t external_ram{};
        work_ram_t work_ram_0{};
        work_ram_t work_ram_1{};
        echo_ram_t echo_ram{};
        oam_memory_t oam{};
        hram_t hram{};
    };

    export using rom_bank_0_page_t = memory::span_map<0x4000>;
    export using rom_bank_n_page_t = memory::span_map<0x4000, 0x4000, 0x7FFF>;
    export using vram_memory_page_t = memory::span_map<graphics::vram_size, graphics::vram_start_address, graphics::vram_end_address>;
    export using external_ram_page_t = memory::span_map<0x2000, 0xA000, 0xBFFF>;
    export using work_ram_0_page_t = memory::span_map<0x1000, 0xC000, 0xCFFF>;
    export using work_ram_1_page_t = memory::span_map<0x1000, 0xD000, 0xDFFF>;
    export using echo_ram_page_t = memory::span_map<0x1E00, 0xE000, 0xFDFF>;

    export using rom_read_only_policy_t = memory::read_only_memory_policy<0x0000, 0x7FFF>;

    // OAM + not-usable memory region
    // TODO: oam corruption bug
    export class oam_memory_page
    {
    public:
        static constexpr memory::memory_address_t start = graphics::oam_start_address;
        static constexpr memory::memory_address_t end = 0xFEFF;

        explicit oam_memory_page(
            const graphics::oam_dma& oam_dma,
            const std::span<memory::memory_data_t, graphics::oam_size> oam)
            : dma{oam_dma}
            , oam { oam }
        {}

        [[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
        {
            return memory::is_in_region<0xFEA0, 0xFEFF>(address)
                ? 0xFF
                : oam[address - start];
        }

        void write(const memory::memory_address_t address, const memory::memory_data_t value)
        {
            if (!memory::is_in_region<0xFEA0, 0xFEFF>(address))
            {
                oam[address - start] = value;
            }
        }

    private:
        const graphics::oam_dma& dma;
        std::span<memory::memory_data_t, graphics::oam_size> oam;
    };

    export class io_hram_interrupt_memory_page
    {
    public:
        static constexpr memory::memory_address_t start = 0xFF00;
        static constexpr memory::memory_address_t end = 0xFFFF;

        io_hram_interrupt_memory_page(
            timer::timer_system& timers,
            interrupts::interrupt_registers& interrupts,
            graphics::pixel_processing_unit& ppu,
            const memory::memory_span_t<0x7F> hram,
            joypad::joypad& joypad,
            graphics::oam_dma& oam_dma)
            : timers{timers}
            , interrupts{interrupts}
            , ppu{ppu}
            , hram { hram }
            , joypad { joypad }
            , oam_dma { oam_dma }
        {}

        [[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
        {
            if (memory::is_in_region<0xFF80, 0xFFFE>(address))
            {
                return hram[address - 0xFF80];
            }

            switch (address)
            {
            case timer::div_address: return static_cast<memory::memory_data_t>(timers.divider() >> 8);
            case timer::tima_address: return timers.counter();
            case timer::tma_address: return timers.modulo().value;
            case timer::tac_address: return timers.control();
            case graphics::lcd_status_address: return graphics::lcd_status(ppu);
            case graphics::lcdc_address: return (ppu.is_enabled() << 7) | fallback_memory[address - start];
            case graphics::lcd_y_address: return ppu.scanline();
            case graphics::lcd_cy_address: return ppu.lyc();
            case graphics::oam_dma_transfer_address: return oam_dma.start_address() / 0x100;
            case interrupts::if_address: return interrupts.flag;
            case interrupts::ie_address: return interrupts.enable;
            case joypad::joypad_memory_address: return joypad::read_joypad_register(joypad);
            default: return fallback_memory[address - start];
            }
        }

        void write(const memory::memory_address_t address, const memory::memory_data_t value)
        {
            if (memory::is_in_region<0xFF80, 0xFFFE>(address))
            {
                hram[address - 0xFF80] = value;
                return;
            }

            switch (address)
            {
            case timer::div_address:
                timers.divider() = 0x00;
                break;
            case timer::tima_address:
                timers.counter() = value;
                timers.cancel_pending_interrupt();
                break;
            case timer::tma_address:
                timers.modulo().value = value;
                break;
            case timer::tac_address:
                timers.control() = value;
                break;
            case graphics::lcd_status_address:
                graphics::set_lcd_status(value, ppu);
                break;
            case graphics::lcdc_address:
                ppu.set_enabled(utils::is_bit_set<7>(value));
                fallback_memory[address - start] = value;
                break;
            case graphics::lcd_y_address: // readonly
                break;
            case graphics::oam_dma_transfer_address:
                oam_dma.start_transfer(value * static_cast<memory::memory_address_t>(0x100));
                break;
            case graphics::lcd_cy_address:
                ppu.set_lyc(value);
                break;
            case interrupts::if_address:
                interrupts.flag = value;
                break;
            case interrupts::ie_address:
                interrupts.enable = value;
                break;
            case joypad::joypad_memory_address:
                joypad::write_joypad_register(joypad, value);
                break;
            default:
                fallback_memory[address - start] = value;
            }
        }

    private:
        timer::timer_system& timers;
        interrupts::interrupt_registers& interrupts;
        graphics::pixel_processing_unit& ppu;
        memory::memory_span_t<0x7F> hram;
        joypad::joypad& joypad;
        graphics::oam_dma& oam_dma;

        // TODO: replace by remaining missing io registers
        std::array<memory::memory_data_t, end - start + 1> fallback_memory{};
    };

    export class memory_map
    {
    public:
        memory_map(
            memory_blocks& memory,
            graphics::oam_dma& oam_dma,
            timer::timer_system& timers,
            interrupts::interrupt_registers& interrupts,
            graphics::pixel_processing_unit& ppu,
            joypad::joypad& joypad)
                : rom_bank_0_page { memory.rom_bank_0 }
                , rom_bank_n_page { memory.rom_bank_n }
                , vram_memory_page { memory.vram }
                , external_ram_page { memory.external_ram }
                , work_ram_0_page { memory.work_ram_0 }
                , work_ram_1_page { memory.work_ram_1 }
                , echo_ram_page { memory.echo_ram }
                , oam_memory_page { oam_dma, memory.oam }
                , ihi_page { timers, interrupts, ppu, memory.hram, joypad, oam_dma }
        {
            map = memory::build_memory_map(
                rom_bank_0_page,
                rom_bank_n_page,
                vram_memory_page,
                external_ram_page,
                work_ram_0_page,
                work_ram_1_page,
                echo_ram_page,
                oam_memory_page,
                ihi_page);
        }

        [[nodiscard]] memory::memory_map_span_t get() { return map; }

    private:
        rom_bank_0_page_t rom_bank_0_page;
        rom_bank_n_page_t rom_bank_n_page;
        vram_memory_page_t vram_memory_page;
        external_ram_page_t external_ram_page;
        work_ram_0_page_t work_ram_0_page;
        work_ram_1_page_t work_ram_1_page;
        echo_ram_page_t echo_ram_page;
        oam_memory_page oam_memory_page;
        io_hram_interrupt_memory_page ihi_page;

        memory::memory_map_array_t map;
    };

    export class memory_buses
    {
    public:
        memory_buses(
            const memory::memory_map_span_t map,
            cpu::cpu_state& cpu,
            timer::timer_system& timers,
            graphics::pixel_processing_unit& ppu,
            graphics::oam_dma& oam_dma,
            joypad::joypad& joypad)
                : vram_policy { ppu }
                , oam_dma_policy { oam_dma  }
                , oam_ppu_policy { ppu, oam_dma  }
                , no_policy_bus { map }
                , cpu_policy_bus { map, vram_policy, oam_dma_policy, oam_ppu_policy, rom_policy }
                , ppu_policy_bus { map, oam_ppu_policy, rom_policy }
        {
            memory::connect(cpu_policy_bus, cpu, timers);
            memory::connect(ppu_policy_bus, ppu);
            memory::connect(no_policy_bus, oam_dma, joypad);
        }

    private:
        rom_read_only_policy_t rom_policy {};
        graphics::vram_access_policy vram_policy;
        graphics::oam_dma_access_policy oam_dma_policy;
        graphics::oam_ppu_access_policy oam_ppu_policy;

        memory::memory_bus no_policy_bus;
        memory::memory_bus cpu_policy_bus;
        memory::memory_bus ppu_policy_bus;
    };

}
