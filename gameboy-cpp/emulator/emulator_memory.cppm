export module emulator.engine:memory;

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
    export using oam_memory_t = std::array<memory::memory_data_t, graphics::oam_size>;
    export using hram_t = std::array<memory::memory_data_t, 0x7F>;

    export struct internal_memory
    {
        vram_t vram{};
        rom_bank_t rom_bank_0{};
        rom_bank_t rom_bank_n{};
        external_ram_t external_ram{};
        work_ram_t work_ram_0{};
        work_ram_t work_ram_1{};
        oam_memory_t oam{};
        hram_t hram{};
    };

    export using rom_bank_0_page_t = memory::span_map<0x4000>;
    export using rom_bank_n_page_t = memory::span_map<0x4000, 0x4000, 0x7FFF>;
    export using vram_memory_page_t = memory::span_map<
        graphics::vram_size, graphics::vram_start_address, graphics::vram_end_address>;
    export using external_ram_page_t = memory::span_map<0x2000, 0xA000, 0xBFFF>;
    export using work_ram_0_page_t = memory::span_map<0x1000, 0xC000, 0xCFFF>;
    export using work_ram_1_page_t = memory::span_map<0x1000, 0xD000, 0xDFFF>;

    export using rom_read_only_policy_t = memory::read_only_memory_policy<0x0000, 0x7FFF>;

    export class echo_ram_memory_page
    {
    public:
        static constexpr memory::memory_address_t start = 0xE000;
        static constexpr memory::memory_address_t end = 0xFDFF;

        explicit echo_ram_memory_page(work_ram_0_page_t& wram_0, work_ram_1_page_t& wram_1)
            : wram_0 { wram_0 }
            , wram_1 { wram_1 }
        {}

        [[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
        {
            using namespace memory;
            const memory_address_t real_address = address - 0x2000;

            return is_in_region<wram_0.start, wram_0.end>(real_address)
                ? wram_0.read(real_address)
                : wram_1.read(real_address);
        }

        void write(const memory::memory_address_t address, const memory::memory_data_t value) const
        {
            using namespace memory;
            const memory_address_t real_address = address - 0x2000;

            if (is_in_region<wram_0.start, wram_0.end>(real_address))
            {
                wram_0.write(real_address, value);
            }
            else
            {
                wram_1.write(real_address, value);
            }
        }

    private:
        work_ram_0_page_t& wram_0;
        work_ram_1_page_t& wram_1;
    };

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
              , oam{oam}
        {
        }

        [[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
        {
            return address >= 0xFEA0 && address <= 0xFEFF
                       ? 0xFF
                       : oam[address - start];
        }

        void write(const memory::memory_address_t address, const memory::memory_data_t value)
        {
            if (!(address >= 0xFEA0 && address <= 0xFEFF))
            {
                oam[address - start] = value;
            }
        }

    private:
        const graphics::oam_dma& dma;
        std::span<memory::memory_data_t, graphics::oam_size> oam;
    };

#define HRAM_CASES(X) \
X(0x00) X(0x01) X(0x02) X(0x03) X(0x04) X(0x05) X(0x06) X(0x07) \
X(0x08) X(0x09) X(0x0A) X(0x0B) X(0x0C) X(0x0D) X(0x0E) X(0x0F) \
X(0x10) X(0x11) X(0x12) X(0x13) X(0x14) X(0x15) X(0x16) X(0x17) \
X(0x18) X(0x19) X(0x1A) X(0x1B) X(0x1C) X(0x1D) X(0x1E) X(0x1F) \
X(0x20) X(0x21) X(0x22) X(0x23) X(0x24) X(0x25) X(0x26) X(0x27) \
X(0x28) X(0x29) X(0x2A) X(0x2B) X(0x2C) X(0x2D) X(0x2E) X(0x2F) \
X(0x30) X(0x31) X(0x32) X(0x33) X(0x34) X(0x35) X(0x36) X(0x37) \
X(0x38) X(0x39) X(0x3A) X(0x3B) X(0x3C) X(0x3D) X(0x3E) X(0x3F) \
X(0x40) X(0x41) X(0x42) X(0x43) X(0x44) X(0x45) X(0x46) X(0x47) \
X(0x48) X(0x49) X(0x4A) X(0x4B) X(0x4C) X(0x4D) X(0x4E) X(0x4F) \
X(0x50) X(0x51) X(0x52) X(0x53) X(0x54) X(0x55) X(0x56) X(0x57) \
X(0x58) X(0x59) X(0x5A) X(0x5B) X(0x5C) X(0x5D) X(0x5E) X(0x5F) \
X(0x60) X(0x61) X(0x62) X(0x63) X(0x64) X(0x65) X(0x66) X(0x67) \
X(0x68) X(0x69) X(0x6A) X(0x6B) X(0x6C) X(0x6D) X(0x6E) X(0x6F) \
X(0x70) X(0x71) X(0x72) X(0x73) X(0x74) X(0x75) X(0x76) X(0x77) \
X(0x78) X(0x79) X(0x7A) X(0x7B) X(0x7C) X(0x7D) X(0x7E)

#define HRAM_READ_CASE(i) \
case (0xFF80 + (i)): return hram[i];

#define HRAM_WRITE_CASE(i) \
case (0xFF80 + (i)): hram[i] = value; break;

#define UNUSED_HW_IO_CASES(X) \
X(0xFF03) X(0xFF08) X(0xFF09) X(0xFF0A) X(0xFF0B) X(0xFF0C) X(0xFF0D) X(0xFF0E) \
X(0xFF15) X(0xFF1F) \
X(0xFF27) X(0xFF28) X(0xFF29) \
X(0xFF4C) X(0xFF4D) X(0xFF4E) X(0xFF4F) \
X(0xFF50) X(0xFF51) X(0xFF52) X(0xFF53) X(0xFF54) X(0xFF55) X(0xFF56) X(0xFF57) X(0xFF58) X(0xFF59) X(0xFF5A) X(0xFF5B) X(0xFF5C) X(0xFF5D) X(0xFF5E) \
X(0xFF5F) X(0xFF60) X(0xFF61) X(0xFF62) X(0xFF63) X(0xFF64) X(0xFF65) X(0xFF66) X(0xFF67) \
X(0xFF68) X(0xFF69) X(0xFF6A) X(0xFF6B) X(0xFF6C) X(0xFF6D) X(0xFF6E) X(0xFF6F) X(0xFF70) X(0xFF71) \
X(0xFF72) X(0xFF73) X(0xFF74) X(0xFF75) X(0xFF76) X(0xFF77) X(0xFF78) X(0xFF79) X(0xFF7A) X(0xFF7B) X(0xFF7C) X(0xFF7D) X(0xFF7E) X(0xFF7F) \

#define UNUSED_HW_IO_READ_CASE(i) \
case i: return 0xFF;

#define UNUSED_HW_IO_WRITE_CASE(i) \
case i: break;

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
              , hram{hram}
              , joypad{joypad}
              , oam_dma{oam_dma}
        {
        }

        [[nodiscard]] memory::memory_data_t read(const memory::memory_address_t address) const
        {
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
            case graphics::oam_dma_transfer_address: return oam_dma.start_address() / static_cast<memory::memory_address_t>(0x100);
            case interrupts::if_address: return interrupts.flag;
            case interrupts::ie_address: return interrupts.enable;
            case joypad::joypad_memory_address: return joypad::read_joypad_register(joypad);
            HRAM_CASES(HRAM_READ_CASE)
            UNUSED_HW_IO_CASES(UNUSED_HW_IO_READ_CASE)
            default: return fallback_memory[address - start];
            }
        }

        void write(const memory::memory_address_t address, const memory::memory_data_t value)
        {
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
            HRAM_CASES(HRAM_WRITE_CASE)
            UNUSED_HW_IO_CASES(UNUSED_HW_IO_WRITE_CASE)
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

    export using memory_map_t = memory::memory_map<
        rom_bank_0_page_t,
        rom_bank_n_page_t,
        vram_memory_page_t,
        external_ram_page_t,
        work_ram_0_page_t,
        work_ram_1_page_t,
        echo_ram_memory_page,
        oam_memory_page,
        io_hram_interrupt_memory_page>;

    export class memory_map
    {
    public:
        memory_map(
            internal_memory& memory,
            graphics::oam_dma& oam_dma,
            timer::timer_system& timers,
            interrupts::interrupt_registers& interrupts,
            graphics::pixel_processing_unit& ppu,
            joypad::joypad& joypad)
            : rom_bank_0_page{memory.rom_bank_0}
              , rom_bank_n_page{memory.rom_bank_n}
              , vram_memory_page{memory.vram}
              , external_ram_page{memory.external_ram}
              , work_ram_0_page{memory.work_ram_0}
              , work_ram_1_page{memory.work_ram_1}
              , echo_ram_page{work_ram_0_page, work_ram_1_page}
              , oam_memory_page{oam_dma, memory.oam}
              , ihi_page{timers, interrupts, ppu, memory.hram, joypad, oam_dma}
              , map{
                  rom_bank_0_page,
                  rom_bank_n_page,
                  vram_memory_page,
                  external_ram_page,
                  work_ram_0_page,
                  work_ram_1_page,
                  echo_ram_page,
                  oam_memory_page,
                  ihi_page
              }
        {
        }

        [[nodiscard]] memory_map_t& get() { return map; }

    private:
        rom_bank_0_page_t rom_bank_0_page;
        rom_bank_n_page_t rom_bank_n_page;
        vram_memory_page_t vram_memory_page;
        external_ram_page_t external_ram_page;
        work_ram_0_page_t work_ram_0_page;
        work_ram_1_page_t work_ram_1_page;
        echo_ram_memory_page echo_ram_page;
        oam_memory_page oam_memory_page;
        io_hram_interrupt_memory_page ihi_page;

        memory_map_t map;
    };

    export using cpu_memory_bus_t = memory::memory_bus<
        memory_map_t,
        graphics::vram_access_policy,
        graphics::oam_dma_access_policy,
        graphics::oam_ppu_access_policy,
        rom_read_only_policy_t>;

    export using timers_memory_bus_t = memory::memory_bus<
        memory_map_t,
        graphics::vram_access_policy,
        graphics::oam_dma_access_policy,
        graphics::oam_ppu_access_policy,
        rom_read_only_policy_t>;

    export using ppu_memory_bus_t = memory::memory_bus<
        memory_map_t,
        rom_read_only_policy_t,
        graphics::oam_ppu_access_policy>;

    export using oam_dma_memory_bus_t = memory::memory_bus<memory_map_t>;

    export class memory_buses
    {
    public:
        memory_buses(
            memory_map_t& map,
            const graphics::pixel_processing_unit& ppu,
            const graphics::oam_dma& oam_dma)
            : vram_policy{ppu}
              , oam_dma_policy{oam_dma}
              , oam_ppu_policy{ppu, oam_dma}
              , cpu_bus_{map, vram_policy, oam_dma_policy, oam_ppu_policy, rom_policy}
              , timers_bus_{map, vram_policy, oam_dma_policy, oam_ppu_policy, rom_policy}
              , ppu_bus_{map, rom_policy, oam_ppu_policy}
              , oam_dma_bus_{map}
        {
        }

        [[nodiscard]] auto& cpu_bus() { return cpu_bus_; }
        [[nodiscard]] auto& timers_bus() { return timers_bus_; }
        [[nodiscard]] auto& ppu_bus() { return ppu_bus_; }
        [[nodiscard]] auto& oam_bus() { return oam_dma_bus_; }

    private:
        rom_read_only_policy_t rom_policy{};
        graphics::vram_access_policy vram_policy;
        graphics::oam_dma_access_policy oam_dma_policy;
        graphics::oam_ppu_access_policy oam_ppu_policy;

        cpu_memory_bus_t cpu_bus_;
        timers_memory_bus_t timers_bus_;
        ppu_memory_bus_t ppu_bus_;
        oam_dma_memory_bus_t oam_dma_bus_;
    };
}
