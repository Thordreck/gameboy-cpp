
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
            constexpr memory::memory_address_t hram_start_address = 0xFF80;
            constexpr memory::memory_address_t hram_end_address = 0xFFFE;

            return memory::is_in_region<hram_start_address, hram_end_address>(address)
                || !dma.is_transfer_active();
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
            constexpr memory::memory_address_t hram_start_address = 0xFF80;
            constexpr memory::memory_address_t hram_end_address = 0xFFFE;

            return memory::is_in_region<hram_start_address, hram_end_address>(address)
                || !dma.is_transfer_active();
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
