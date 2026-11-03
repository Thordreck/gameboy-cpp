
export module graphics:memory;

import std;

import memory;
import utilities;
import interrupts;

import :ppu;
import :oam;
import :vram;
import :window;
import :objects;
import :background;

namespace graphics
{
    export class dma_access_policy
    {
    public:
        explicit dma_access_policy(const oam_dma& oam_dma)
            : dma { oam_dma }
        {}

        [[nodiscard]] bool can_read(const memory::memory_address_t address) const
        {
            return !check_bus_conflict(dma, address);
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
            return !check_bus_conflict(dma, address);
        }

    private:
        static bool check_bus_conflict(const oam_dma& dma, const memory::memory_address_t address)
        {
            if (!dma.is_transfer_active())
            {
                return false;
            }

            using namespace memory;

            if (is_in_region<oam_start_address, oam_end_address>(address))
            {
                return true;
            }

            if (is_in_region<vram_start_address, vram_end_address>(dma.active_start_address())
                && is_in_region<vram_start_address, vram_end_address>(address))
            {
                return true;
            }

            const bool is_dma_in_main_bus
                = is_in_region<0x0000, 0x7FFF>(dma.active_start_address())
                || is_in_region<0xA000, 0xFDFF>(dma.active_start_address());

            const bool is_target_in_main_bus
                = is_in_region<0x0000, 0x7FFF>(address)
                || is_in_region<0xA000, 0xFDFF>(address);

            if (is_dma_in_main_bus && is_target_in_main_bus)
            {
                return true;
            }

            return false;
        }

        const oam_dma& dma;
    };

    export class oam_access_policy
    {
    public:
        explicit oam_access_policy(const pixel_processing_unit& ppu)
            : ppu { ppu }
        {}

        [[nodiscard]] bool can_read(const memory::memory_address_t address) const
        {
            return !(address >= oam_start_address && address <= oam_end_address)
                || !ppu.is_enabled()
                || (ppu.mode() == ppu_mode::v_blank || ppu.mode() == ppu_mode::h_blank) && ppu.current_scanline_cycle() < 452;
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
            return !(address >= oam_start_address && address <= oam_end_address)
                || !ppu.is_enabled()
                || (ppu.mode() == ppu_mode::v_blank || ppu.mode() == ppu_mode::h_blank)
                || (ppu.mode() == ppu_mode::oam_scan && ppu.current_scanline_cycle() >= 76)
                || (ppu.mode() == ppu_mode::drawing && ppu.current_scanline_cycle() == 80);
        }

    private:
        const pixel_processing_unit& ppu;
    };

    // lcdc
    export void write_lcdc_register(
        pixel_processing_unit& ppu,
        const memory::memory_data_t value,
        interrupts::interrupt_controller& interrupts)
    {
        using namespace utils;

        using enum object_size;
        using enum window_tilemap_address_mode;
        using enum background_tile_data_area;

        ppu.set_enabled(is_bit_set<7>(value), interrupts);
        ppu.set_window_tilemap_area(is_bit_set<6>(value) ? _0x9C00 : _0x9800);
        ppu.set_window_enabled(is_bit_set<5>(value));
        ppu.set_background_tile_data_area(is_bit_set<4>(value) ? _0x8000 : _0x8800);
        ppu.set_background_tilemap_area(is_bit_set<3>(value) ? background_tilemap_area::_0x9C00 : background_tilemap_area::_0x9800);
        ppu.set_objects_size(is_bit_set<2>(value) ? _8x16 : _8x8);
        ppu.set_objects_enabled(is_bit_set<1>(value));
        ppu.set_background_and_window_display_enabled(is_bit_set<0>(value));
    }

    export [[nodiscard]] memory::memory_data_t read_lcdc_register(const pixel_processing_unit& ppu)
    {
        using enum object_size;
        using enum window_tilemap_address_mode;
        using enum background_tile_data_area;

        return (ppu.is_enabled() << 7)
            | ((ppu.get_window_tilemap_area() == _0x9C00) << 6)
            | (ppu.get_window_enabled() << 5)
            | ((ppu.get_background_tile_data_area() == _0x8000) << 4)
            | ((ppu.get_background_tilemap_area() == background_tilemap_area::_0x9C00) << 3)
            | ((ppu.get_objects_size() == _8x16) << 2)
            | (ppu.get_objects_enabled() << 1)
            | (ppu.get_background_and_window_display_enabled() << 0);
    }
}
