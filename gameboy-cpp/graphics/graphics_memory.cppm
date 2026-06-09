
export module graphics:memory;

export import :ppu;
export import :oam;
export import :vram;
export import memory;
export import std;

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
                || !(ppu.mode() == ppu_mode::oam_scan || ppu.mode() == ppu_mode::drawing);
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
		    return !(address >= oam_start_address && address <= oam_end_address)
                || !ppu.is_enabled()
                || !(ppu.mode() == ppu_mode::oam_scan || ppu.mode() == ppu_mode::drawing);
        }

    private:
        const pixel_processing_unit& ppu;
    };

}
