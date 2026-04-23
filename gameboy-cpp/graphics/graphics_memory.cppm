
export module graphics:memory;

export import :ppu;
export import :oam;
export import memory;
export import std;

namespace graphics
{
    export class oam_dma_access_policy
    {
    public:
        explicit oam_dma_access_policy(const oam_dma& oam_dma)
            : dma { oam_dma }
        {}

        [[nodiscard]] bool can_read(const memory::memory_address_t address) const
        {
            if (!dma.is_transfer_active())
            {
                return true;
            }

            using namespace memory;

            return !(is_in_region<0x0000, 0x7FFF>(address) && is_in_region<0x0000, 0x7FFF>(dma.start_address()))
                && !(is_in_region<0x8000, 0x9FFF>(address) && is_in_region<0x8000, 0x9FFF>(dma.start_address()))
                && !(is_in_region<0xA000, 0xBFFF>(address) && is_in_region<0xA000, 0xBFFF>(dma.start_address()));
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
            if (!dma.is_transfer_active())
            {
                return true;
            }

            using namespace memory;

            return !(is_in_region<0x0000, 0x7FFF>(address) && is_in_region<0x0000, 0x7FFF>(dma.start_address()))
                && !(is_in_region<0x8000, 0x9FFF>(address) && is_in_region<0x8000, 0x9FFF>(dma.start_address()))
                && !(is_in_region<0xA000, 0xBFFF>(address) && is_in_region<0xA000, 0xBFFF>(dma.start_address()));
        }

    private:
        const oam_dma& dma;
    };

    export class oam_ppu_access_policy
    {
    public:
        explicit oam_ppu_access_policy(const pixel_processing_unit& ppu, const oam_dma& oam_dma)
            : ppu { ppu }
        , dma { oam_dma }
        {}

        [[nodiscard]] bool can_read(const memory::memory_address_t address) const
        {
		    return !(address >= oam_start_address && address <= oam_end_address)
                || !(ppu.mode() == ppu_mode::oam_scan || ppu.mode() == ppu_mode::drawing)
                || !dma.is_transfer_active();
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
		    return !(address >= oam_start_address && address <= oam_end_address)
                || !(ppu.mode() == ppu_mode::oam_scan || ppu.mode() == ppu_mode::drawing)
                || !dma.is_transfer_active();
        }

    private:
        const pixel_processing_unit& ppu;
        const oam_dma& dma;
    };

}
