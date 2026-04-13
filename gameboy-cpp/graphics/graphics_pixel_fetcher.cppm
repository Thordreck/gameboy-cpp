export module graphics:pixel_fetcher;

export import :fifo;
export import memory;
export import :oam;

import :registers;
import utilities;

namespace graphics
{
    template<memory::ReadOnlyMemory Memory>
    std::uint8_t compute_fetcher_y(const Memory& memory,
                                   const bool is_window_tile,
                                   const std::uint8_t window_line)
    {
        return is_window_tile
                   ? window_line
                   : lcd_y(memory) + scy(memory);
    }

    std::uint8_t compute_tile_y(const std::uint8_t fetcher_y)
    {
        return (fetcher_y / 8) & 0x1F;
    }

    std::uint8_t compute_tile_line(const std::uint8_t fetcher_y)
    {
        return fetcher_y % 8;
    }

    template<memory::ReadOnlyMemory Memory>
    std::uint8_t compute_tile_x(
        const Memory& memory,
        const std::uint8_t fetcher_x,
        const bool is_window_tile)
    {
        return is_window_tile ? fetcher_x : ((scx(memory) / 8) + fetcher_x) & 0x1F;
    }

    template<memory::ReadOnlyMemory Memory>
    std::uint16_t compute_tilemap_base_address(const Memory& memory, const bool is_window_tile)
    {
        return is_window_tile
                   ? is_window_tilemap_flag_set(memory)
                         ? 0x9C00
                         : 0x9800
                   : is_background_tilemap_flag_set(memory)
                   ? 0x9C00
                   : 0x9800;
    }

    template<memory::ReadOnlyMemory Memory>
    std::uint16_t compute_base_tile_data_address(const Memory& memory)
    {
        return is_addressing_mode_flag_set(memory) ? 0x8000 : 0x9000;
    }

    template<memory::ReadOnlyMemory Memory>
    std::uint16_t compute_tile_address(const Memory& memory, const std::uint8_t tile_index, const std::uint8_t tile_line)
    {
        const std::uint16_t tile_base = compute_base_tile_data_address(memory);
        const bool unsigned_address_mode = is_addressing_mode_flag_set(memory);

        return unsigned_address_mode
                   ? tile_base + tile_index * 16 + tile_line * 2
                   : tile_base + static_cast<std::int8_t>(tile_index) * 16 + tile_line * 2;
    }

    color_index_t decode_tile(const int index, const std::uint8_t low, const std::uint8_t high)
    {
        return (high >> index & 1) << 1 | low >> index & 1;
    }

    template<memory::ReadOnlyMemory Memory>
    void decode_background_tile_row(
        const Memory& memory,
        const std::uint8_t low,
        const std::uint8_t high,
        std::span<pixel, 8> pixels)
    {
        const bool is_bg_and_window_enabled = is_bg_and_window_display_flag_set(memory);

        for (int i = 7; i >= 0; i--)
        {
            const color_index_t color = is_bg_and_window_enabled
                ? decode_tile(i, low, high)
                : 0;

            pixels[7 - i] = {color};
        }
    }

    void decode_sprite_tile_row(
        const object& object,
        const std::uint8_t low,
        const std::uint8_t high,
        std::span<pixel, 8> pixels)
    {
        for (int i = 7; i >= 0; i--)
        {
            const color_index_t color = decode_tile(i, low, high);
            const std::uint8_t palette_index = object.alternate_palette() ? 1 : 0;
            const bool background_priority = object.priority();

            pixels[7 - i] = { color, palette_index, background_priority, object.x(), object.index() };
        }
    }

    template<memory::ReadOnlyMemory Memory>
    std::uint8_t compute_sprite_tile_line(const object& object, const Memory& memory)
    {
        const std::uint8_t tile_line = lcd_y(memory) - (object.y() - 16);

        return object.y_flip()
            ? get_objects_height(memory) - 1 - tile_line
            : tile_line;
    }

    template<memory::ReadOnlyMemory Memory>
    std::uint8_t compute_sprite_tile_index(const object& object, const std::uint8_t tile_line, Memory& memory)
    {
        const std::uint8_t sprite_height = get_objects_height(memory);

        return sprite_height < 16
            ? object.tile_index()
            : tile_line < 8
                ? object.tile_index() & 0xFE
                : object.tile_index() | 0x01;
    }

    pixel resolve_drawing_priority(const pixel& lhs, const pixel& rhs)
    {
        // First, check opacity
        if (lhs.color_index == 0)
        {
            return rhs;
        }

        if (rhs.color_index == 0)
        {
            return lhs;
        }

        const std::uint8_t lhs_x = lhs.x.value_or(0);
        const std::uint8_t rhs_x = rhs.x.value_or(0);

        if (lhs_x < rhs_x)
        {
            return lhs;
        }

        if (rhs_x < lhs_x)
        {
            return rhs;
        }

        const std::uint8_t lhs_oam_index = lhs.oam_index.value_or(0);
        const std::uint8_t rhs_oam_index = rhs.oam_index.value_or(0);

        return lhs_oam_index < rhs_oam_index ? lhs : rhs;
    }

    enum class pixel_fetch_step : std::uint8_t
    {
        get_tile_address,
        get_tile_data_low,
        get_tile_data_high,
        sleep,
        push,
    };

    export class pixel_fetcher
    {
    public:
        explicit pixel_fetcher(pixel_fifo& fifo)
            : fifo{fifo}
              , current_step{pixel_fetch_step::get_tile_address}
        {
        }

        void reset()
        {
            fetcher_x = 0;
            current_step = pixel_fetch_step::get_tile_address;
            step_cycle = 0;
        }

        template<memory::ReadOnlyMemory Memory>
        void tick(const bool is_window_active, const std::uint8_t window_line, const Memory& memory)
        {
            switch (current_step)
            {
            case pixel_fetch_step::get_tile_address:
                fetch_tile_address(is_window_active, window_line, memory);
                break;
            case pixel_fetch_step::get_tile_data_low:
                get_tile_data_low(memory);
                break;
            case pixel_fetch_step::get_tile_data_high:
                get_tile_data_high(memory);
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

    private:
        template<memory::ReadOnlyMemory Memory>
        void fetch_tile_address(const bool is_window_active, const std::uint8_t window_line, const Memory& memory)
        {
            if (++step_cycle < 2)
            {
                return;
            }

            const std::uint8_t fetcher_y = compute_fetcher_y(memory, is_window_active, window_line);

            const std::uint8_t tile_x = compute_tile_x(memory, fetcher_x, is_window_active);
            const std::uint8_t tile_y = compute_tile_y(fetcher_y);
            const std::uint8_t tile_line = compute_tile_line(fetcher_y);

            const std::uint16_t tilemap_base_address = compute_tilemap_base_address(memory, is_window_active);
            const std::uint16_t tilemap_address = tilemap_base_address + tile_y * 32 + tile_x;
            const std::uint8_t tile_index = memory.read(tilemap_address);

            tile_address = compute_tile_address(memory, tile_index, tile_line);
            advance_state();
        }

        template<memory::ReadOnlyMemory Memory>
        void get_tile_data_low(const Memory& memory)
        {
            if (++step_cycle < 2)
            {
                return;
            }

            tile_low_byte = memory.read(tile_address);
            advance_state();
        }

        template<memory::ReadOnlyMemory Memory>
        void get_tile_data_high(const Memory& memory)
        {
            if (++step_cycle < 2)
            {
                return;
            }

            const std::uint8_t high_byte = memory.read(tile_address + 1);
            decode_background_tile_row(memory, tile_low_byte, high_byte, tile_row);
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

        pixel_fifo& fifo;

        // Tile coordinate along the scanline (groups of 8 pixels)
        std::uint8_t fetcher_x{};

        pixel_fetch_step current_step;
        std::uint8_t step_cycle{};

        std::uint16_t tile_address{};
        std::uint8_t tile_low_byte{};
        std::array<pixel, 8> tile_row{};
    };

    enum class sprite_fetch_step : std::uint8_t
    {
        get_tile_address,
        get_tile_data_low,
        get_tile_data_high,
    };

    export class object_fetcher
    {
    public:
        explicit object_fetcher(pixel_fifo& fifo)
            : fifo{ fifo }
            , current_step { sprite_fetch_step::get_tile_address }
            , current_target { std::nullopt }
        {}

        [[nodiscard]] bool is_fetching() const { return current_target.has_value(); }

        void set_target(const object& new_target)
        {
            reset();
            current_target = new_target;
        }

        void reset()
        {
            current_step = sprite_fetch_step::get_tile_address;
            step_cycle = 0;
            current_target = std::nullopt;
        }

        template<memory::ReadOnlyMemory Memory>
        void tick(const Memory& memory)
        {
            if (!current_target.has_value())
            {
                return;
            }

            switch (current_step)
            {
            case sprite_fetch_step::get_tile_address:
                fetch_tile_address(current_target.value(), memory);
                break;
            case sprite_fetch_step::get_tile_data_low:
                get_tile_data_low(memory);
                break;
            case sprite_fetch_step::get_tile_data_high:
                get_tile_data_high(current_target.value(), memory);
                break;
            default: std::unreachable();
            }
        }

    private:
        template<memory::ReadOnlyMemory Memory>
        void fetch_tile_address(const object& target, const Memory& memory)
        {
            if (++step_cycle < 2)
            {
                return;
            }

            constexpr memory::memory_address_t tile_base = 0x8000;
            const std::uint8_t tile_line = compute_sprite_tile_line(target, memory);
            const std::uint8_t tile_index = compute_sprite_tile_index(target, tile_line, memory);

            tile_address = tile_base + tile_index * 16 + tile_line * 2;
            advance_state();
        }

        template<memory::ReadOnlyMemory Memory>
        void get_tile_data_low(const Memory& memory)
        {
            if (++step_cycle < 2)
            {
                return;
            }

            tile_low_byte = memory.read(tile_address);
            advance_state();
        }

        template<memory::ReadOnlyMemory Memory>
        void get_tile_data_high(const object& target, const Memory& memory)
        {
            if (++step_cycle < 2)
            {
                return;
            }

            const std::uint8_t high_byte = memory.read(tile_address + 1);
            std::array<pixel, 8> tile_row{};
            decode_sprite_tile_row(target, tile_low_byte, high_byte, tile_row);

            if (target.x_flip())
            {
                std::ranges::reverse(tile_row);
            }

            // Ensure there are at least 8 pixels
            while (fifo.count() < 8)
            {
                constexpr pixel transparent_pixel { 0 };
                std::ignore = fifo.try_push(transparent_pixel);
            }

            // Resolve opaque pixel drawing priority
            for (int i = 0; i < tile_row.size(); ++i)
            {
                const pixel new_pixel = tile_row[i];

                tile_row[i] = fifo.try_pop()
                    .transform(
                        [new_pixel] (const auto& prev_pixel) { return resolve_drawing_priority(prev_pixel, new_pixel); })
                    .value_or(new_pixel);
            }

            std::ignore = fifo.try_push(tile_row);
            current_target.reset();
            advance_state();
        }

        void advance_state()
        {
            const auto next = (std::to_underlying(current_step) + 1) % 3;
            current_step = static_cast<sprite_fetch_step>(next);
            step_cycle = 0;
        }

        pixel_fifo& fifo;

        sprite_fetch_step current_step;
        std::uint8_t step_cycle{};

        std::optional<object> current_target;
        std::uint16_t tile_address{};
        std::uint8_t tile_low_byte{};
    };

}
