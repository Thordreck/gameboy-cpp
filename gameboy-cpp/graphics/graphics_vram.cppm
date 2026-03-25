export module graphics:vram;

export import :ppu;
export import memory;
export import std;

namespace graphics
{
    export constexpr memory::memory_address_t vram_start_address = 0x8000;
    export constexpr memory::memory_address_t vram_end_address = 0x9FFF;
    export constexpr memory::memory_address_t vram_size = vram_end_address - vram_start_address + 1;

    export class vram_access_policy
    {
    public:
        vram_access_policy(const pixel_processing_unit& ppu)
            : ppu{ppu}
        {}

        [[nodiscard]] bool can_read(const memory::memory_address_t address) const
        {
            return !memory::is_in_region<vram_start_address, vram_end_address>(address)
                || ppu.mode() != ppu_mode::drawing;
        }

        [[nodiscard]] bool can_write(const memory::memory_address_t address) const
        {
            return !memory::is_in_region<vram_start_address, vram_end_address>(address)
                || ppu.mode() != ppu_mode::drawing;
        }

    private:
        const pixel_processing_unit& ppu;
    };

    export using vram_t = std::span<memory::memory_data_t, vram_size>;
    export using const_vram_t = std::span<const memory::memory_data_t, vram_size>;

    export [[nodiscard]] memory::memory_data_t read_vram(const_vram_t vram, const memory::memory_address_t address)
    {
        return vram[address - vram_start_address];
    }

    export void write_vram(vram_t vram, const memory::memory_address_t address, const memory::memory_data_t value)
    {
        vram[address - vram_start_address] = value;
    }
}
