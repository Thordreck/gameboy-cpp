export module graphics:pixel_fetcher;

export import :vram;
export import :fifo;
export import memory;

import :registers;
import utilities;

namespace
{
    std::uint8_t compute_fetcher_y(const memory::memory_bus& memory)
    {
        using namespace graphics;
        return lcd_y(memory) + scy(memory);
    }

    std::uint8_t compute_tile_y(const std::uint8_t fetcher_y)
    {
        return (fetcher_y / 8) & 0x1F;
    }

    std::uint8_t compute_tile_line(const std::uint8_t fetcher_y)
    {
        return fetcher_y % 8;
    }

    std::uint8_t compute_tile_x(const memory::memory_bus& memory, const std::uint8_t fetcher_x)
    {
        using namespace graphics;
        return ((scx(memory) / 8) + fetcher_x) & 0x1F;
    }

    std::uint16_t compute_tilemap_base_address(const memory::memory_bus& memory)
    {
        using namespace graphics;
        return is_background_tilemap_flag_set(memory) ? 0x9C00 : 0x9800;
    }

    std::uint16_t compute_base_tile_data_address(const memory::memory_bus& memory)
    {
        using namespace graphics;
        return is_addressing_mode_flag_set(memory) ? 0x8000 : 0x9000;
    }

    std::uint16_t compute_tile_address(
        const memory::memory_bus& memory,
        const std::uint8_t tile_index,
        const std::uint8_t tile_line)
    {
        using namespace graphics;

        const std::uint16_t tile_base = compute_base_tile_data_address(memory);
        const bool unsigned_address_mode = is_addressing_mode_flag_set(memory);

        return unsigned_address_mode
                   ? tile_base + tile_index * 16 + tile_line * 2
                   : tile_base + static_cast<std::int8_t>(tile_index) * 16 + tile_line * 2;
    }

    void decode_tile_row(
        const std::uint8_t low,
        const std::uint8_t high,
        std::span<graphics::pixel, 8> pixels)
    {
        using namespace graphics;

        for (int i = 7; i >= 0; i--)
        {
            const color_index_t color = (high >> i & 1) << 1 | low >> i & 1;
            pixels[7 - i] = {color};
        }
    }
}

namespace graphics
{
    enum class pixel_fetch_step : std::uint8_t
    {
        get_tile_address,
        get_tile_data_low,
        get_tile_data_high,
        sleep,
        push,
    };

    // TODO: window support
    export class pixel_fetcher
    {
    public:
        explicit pixel_fetcher(pixel_fifo& fifo, const_vram_t vram)
            : fifo{fifo}
            , vram{vram}
            , current_step{pixel_fetch_step::get_tile_address}
        {}

        void reset_scanline()
        {
            utils::assert_not_nullptr(memory);

            fetcher_x = 0;
            fetcher_y = compute_fetcher_y(*memory);
            current_step = pixel_fetch_step::get_tile_address;
        }

        void tick()
        {
            utils::assert_not_nullptr(memory);

            switch (current_step)
            {
            case pixel_fetch_step::get_tile_address:
                fetch_tile_address();
                break;
            case pixel_fetch_step::get_tile_data_low:
                get_tile_data_low();
                break;
            case pixel_fetch_step::get_tile_data_high:
                get_tile_data_high();
                break;
            case pixel_fetch_step::sleep:
                sleep();
                break;
            case pixel_fetch_step::push:
                push_tile_row();
                break;
            default: std::unreachable();
            }
        }

        void connect(memory::memory_bus& bus) { memory = &bus; }

    private:
        void fetch_tile_address()
        {
            if (++step_cycle < 2)
            {
                return;
            }

            const std::uint8_t tile_x = compute_tile_x(*memory, fetcher_x);
            const std::uint8_t tile_y = compute_tile_y(fetcher_y);
            const std::uint8_t tile_line = compute_tile_line(fetcher_y);

            const std::uint16_t tilemap_base_address = compute_tilemap_base_address(*memory);
            const std::uint16_t tilemap_address = tilemap_base_address + tile_y * 32 + tile_x;
            const std::uint8_t tile_index = read_vram(vram, tilemap_address);

            tile_address = compute_tile_address(*memory, tile_index, tile_line);
            advance_state();
        }

        void get_tile_data_low()
        {
            if (++step_cycle < 2)
            {
                return;
            }

            tile_low_byte = read_vram(vram, tile_address);
            advance_state();
        }

        void get_tile_data_high()
        {
            if (++step_cycle < 2)
            {
                return;
            }

            const std::uint8_t high_byte = read_vram(vram, tile_address + 1);
            decode_tile_row(tile_low_byte, high_byte, tile_row);
            advance_state();

            // TODO: according to pandocs, there's an additional push here, but the wording used
            // was not clear. I'm not sure how it's supposed to be done.
        }

        void sleep()
        {
            if (++step_cycle < 2)
            {
                return;
            }

            advance_state();
        }

        void push_tile_row()
        {
            if (fifo.try_push(tile_row))
            {
                fetcher_x++;
                advance_state();
            }
        }

        void advance_state()
        {
            const auto next = (std::to_underlying(current_step) + 1) % 5;
            current_step = static_cast<pixel_fetch_step>(next);

            step_cycle = 0;
        }

    private:
        pixel_fifo& fifo;
        const_vram_t vram;

		memory::memory_bus* memory { nullptr };

        std::uint8_t fetcher_x {};
        std::uint8_t fetcher_y {};

        pixel_fetch_step current_step;
        std::uint8_t step_cycle{};

        std::uint16_t tile_address{};
        std::uint8_t tile_low_byte{};
        std::array<pixel, 8> tile_row{};
    };
}
