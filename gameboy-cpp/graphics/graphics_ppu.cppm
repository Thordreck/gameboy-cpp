module;
#include "profiling.hpp"

export module graphics:ppu;
export import memory;
export import :lcd;

import std;
import interrupts;
import utilities;

import :fifo;
import :common;
import :pixel_fetcher;
import :oam;
import :object_buffer;

namespace graphics
{
    template<memory::ReadOnlyMemory Memory>
    bool is_window_present_in_scanline(const Memory& memory)
    {
        return lcd_y(memory) >= wy(memory);
    }

    template<memory::ReadOnlyMemory Memory>
    bool is_window_visible(const Memory& memory, const std::uint8_t screen_x)
    {
        return is_window_present_in_scanline(memory) && screen_x >= static_cast<int>(wx(memory)) - 7;
    }

    template<memory::ReadOnlyMemory Memory>
    bool is_window_first_line(const Memory& memory)
    {
        return lcd_y(memory) == wy(memory);
    }

    template<memory::ReadOnlyMemory Memory>
    const pixel& mix(const pixel& background, const pixel& sprite, const Memory& memory)
    {
        const bool bg_and_windows_enabled = is_bg_and_window_display_flag_set(memory);
        const bool objects_enabled = is_obj_enabled(memory);

        // Sprites with color index zero are transparent.
        if (sprite.color_index == 0)
        {
            return background;
        }

        if (!objects_enabled)
        {
            return background;
        }

        if (!bg_and_windows_enabled)
        {
            return sprite;
        }

        if (background.color_index == 0)
        {
            return sprite;
        }

        if (sprite.background_priority && background.color_index != 0)
        {
            return background;
        }

        return sprite;
    }

    memory::memory_address_t get_palette_address(const pixel& pixel)
    {
        return pixel.palette_index
            .transform([] (const auto index) { return index == 1 ? obp1_address : obp0_address; })
            .value_or(bgp_address);
    }

    template<memory::ReadOnlyMemory Memory>
    color get_pixel_color(const pixel& pixel, const Memory& memory, const lcd_color_palette_t palette)
    {
        using namespace memory;

        const memory_address_t palette_address = get_palette_address(pixel);
        const memory_data_t palette_data = memory.read(palette_address);
        const std::uint8_t mapped_color_index = (palette_data >> (pixel.color_index * 2)) & 0b11;

        return palette[mapped_color_index];
    }

    export enum class ppu_mode : std::uint8_t
    {
        h_blank,
        v_blank,
        oam_scan,
        drawing,
    };

    export struct ppu_interrupt_sources
    {
        bool lyc_select;
        bool mode2;
        bool mode1;
        bool mode0;
    };

    export class pixel_processing_unit
    {
    public:
        explicit pixel_processing_unit(lcd& screen)
            : enabled{true}
            , screen(screen)
            , pixel_fetcher(background_fifo)
            , sprite_fetcher(sprite_fifo)
        {}

        [[nodiscard]] std::uint8_t scanline() const { return current_scanline; }
        [[nodiscard]] std::uint8_t lyc() const { return scanline_compare; }
        [[nodiscard]] ppu_mode mode() const { return current_mode; }
        [[nodiscard]] bool is_enabled() const { return enabled; }
        [[nodiscard]] ppu_interrupt_sources interrupts() const { return interrupt_sources; }

        [[nodiscard]] std::uint8_t active() const { return is_enabled(); }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            if (!active()) [[unlikely]] { return std::numeric_limits<std::uint32_t>::max(); }

            switch (current_mode)
            {
            case ppu_mode::h_blank: return 456 - scanline_cycle;
            case ppu_mode::v_blank: return 456 - scanline_cycle;
            case ppu_mode::oam_scan: return 80 - scanline_cycle;
            case ppu_mode::drawing: return 1;
            default: std::unreachable();
            }
        }

        template<memory::Memory Memory>
        void tick(const std::uint32_t num_ticks, Memory& memory)
        {
            PROFILER_SCOPE("PPU::tick()");

            if (!enabled) [[unlikely]] { return; }

            std::uint32_t remaining_ticks = num_ticks;

            while (remaining_ticks-- > 0)
            {
                switch (current_mode)
                {
                case ppu_mode::h_blank:
                    h_blank(memory);
                    break;
                case ppu_mode::v_blank:
                    v_blank(memory);
                    break;
                case ppu_mode::oam_scan:
                    scan_oam(memory);
                    break;
                case ppu_mode::drawing:
                    draw(memory);
                    break;
                default: std::unreachable();
                }

                update_stat_line(memory);
            }
        }

        void set_enabled(const bool enabled)
        {
            this->enabled = enabled;

            if (!enabled)
            {
                current_scanline = 0;
                scanline_cycle = 0;
                window_line = 0;
                window_fetcher_penalty = 0;
                window_active_in_scanline = false;
                pixels_drawn_in_scanline = 0;
                pixels_to_discard = 0;
                background_fifo.clear();
                pixel_fetcher.reset();

                current_mode = ppu_mode::oam_scan;
            }
        }

        void set_interrupts(const ppu_interrupt_sources new_config)
        {
            interrupt_sources = new_config;

            // TODO: re-enable when interrupts do not need access to memory
            //update_stat_line();
        }

        void set_lyc(const std::uint8_t lyc)
        {
            scanline_compare = lyc;

            // TODO: re-enable when interrupts do not need access to memory
            //update_stat_line();
        }

    private:
	    template<memory::Memory Memory>
        void scan_oam(Memory& memory)
        {
            if (scanline_cycle == 0)
            {
                sprite_buffer.clear();
            }

            if (scanline_cycle % 2 == 0)
            {
                const std::uint8_t object_index = scanline_cycle / 2;
                const object candidate = get_object(object_index, memory);
                const std::uint8_t objects_height = get_objects_height(memory);
                const bool is_visible = is_in_scanline(candidate, objects_height, current_scanline);

                if (is_visible && !sprite_buffer.is_full())
                {
                    sprite_buffer.push_back(candidate);
                }
            }

            if (++scanline_cycle >= 80)
            {
                update_mode(ppu_mode::drawing, memory);
            }
        }

	    template<memory::Memory Memory>
        void draw(Memory& memory)
        {
            // Draw start
            if (scanline_cycle++ == 80)
            {
                pixel_fetcher.reset();
                background_fifo.clear();

                sprite_fetcher.reset();
                sprite_fifo.clear();

                pixels_drawn_in_scanline = 0;
                pixels_to_discard = scx(memory) % 8;
            }

            // Window start
            if (!window_active_in_scanline
                && is_window_enabled(memory)
                && is_window_visible(memory, pixels_drawn_in_scanline))
            {
                window_active_in_scanline = true;
                window_line = is_window_first_line(memory) ? 0 : window_line;

                background_fifo.clear();
                pixel_fetcher.reset();
                pixels_to_discard = 0;
                window_fetcher_penalty = wx(memory) > 0 ? 6 : 0;
            }

            if (window_fetcher_penalty > 0)
            {
                window_fetcher_penalty--;
                return;
            }

            // Sprite fetch
            const bool objects_enabled = is_obj_enabled(memory);

            // TODO: sprite fetch cancelling timing
            if (!objects_enabled)
            {
                sprite_fetcher.reset();
                sprite_fifo.clear();
            }

            if (objects_enabled && !sprite_fetcher.is_fetching())
            {
                if (const std::optional next_object = get_object_at_x(sprite_buffer, pixels_drawn_in_scanline); next_object.has_value())
                {
                    sprite_fetcher.set_target(next_object.value());
                }
            }

            // Wait for background fifo to reach step 5
            if (sprite_fetcher.is_fetching() && background_fifo.count() < 8)
            {
                pixel_fetcher.tick(window_active_in_scanline, window_line, memory);
                return;
            }

            // Wait until sprite fetch completes
            sprite_fetcher.tick(memory);

            if (sprite_fetcher.is_fetching())
            {
                return;
            }

            // Background/window fetch
            pixel_fetcher.tick(window_active_in_scanline, window_line, memory);

            if (const std::optional<pixel> bg_pixel = background_fifo.try_pop(); bg_pixel.has_value())
            {
                if (pixels_to_discard > 0)
                {
                    pixels_to_discard--;
                    return;
                }

                const std::optional sprite_pixel = sprite_fifo.try_pop();

                const pixel& mixed_pixel = sprite_pixel.has_value()
                    ? mix(bg_pixel.value(), sprite_pixel.value(), memory)
                    : bg_pixel.value();

                // TODO: make palette configurable
                const color pixel_color = get_pixel_color(mixed_pixel, memory, grayscale_lcd_color_palette);
                const coords_2d pixel_coords { pixels_drawn_in_scanline, current_scanline };

                screen.set_pixel(pixel_coords, pixel_color);
                pixels_drawn_in_scanline++;
            }

            if (pixels_drawn_in_scanline == 160)
            {
                update_mode(ppu_mode::h_blank, memory);
            }
        }

	    template<memory::Memory Memory>
        void h_blank(Memory& memory)
        {
            // End of scanline
            if (++scanline_cycle >= 456)
            {
                scanline_cycle = 0;
                current_scanline++;

                if (window_active_in_scanline)
                {
                    window_active_in_scanline = false;
                    window_line++;
                }

                const auto next_mode = current_scanline <= 143
                                           ? ppu_mode::oam_scan
                                           : ppu_mode::v_blank;

                update_mode(next_mode, memory);
            }
        }

	    template<memory::Memory Memory>
        void v_blank(Memory& memory)
        {
            if (++scanline_cycle >= 456)
            {
                scanline_cycle = 0;
                current_scanline++;

                if (current_scanline > 153)
                {
                    current_scanline = 0;
                    update_mode(ppu_mode::oam_scan, memory);
                }
            }
        }

	    template<memory::Memory Memory>
        void update_stat_line(Memory& memory)
        {
            if (!enabled) [[unlikely]] { return; }

            const bool new_stat_line
                = (current_scanline == scanline_compare && interrupt_sources.lyc_select)
                || (current_mode == ppu_mode::h_blank && interrupt_sources.mode0)
                || (current_mode == ppu_mode::v_blank && interrupt_sources.mode1)
                || (current_mode == ppu_mode::oam_scan && interrupt_sources.mode2);

            const bool should_trigger_stat_interrupt
                = !stat_line && new_stat_line;

            if (should_trigger_stat_interrupt)
            {
                using namespace interrupts;
                request(lcd_interrupt, memory);
            }

            stat_line = new_stat_line;
        }

	    template<memory::Memory Memory>
        void update_mode(const ppu_mode new_mode, Memory& memory)
        {
            current_mode = new_mode;

            // VBlank interrupt
            if (new_mode == ppu_mode::v_blank)
            {
                using namespace interrupts;
                request(vblank_interrupt, memory);
            }
        }

    private:
        bool enabled;
        ppu_mode current_mode{ppu_mode::oam_scan};
        std::uint8_t scanline_compare{};
        std::uint8_t current_scanline{};
        std::uint16_t scanline_cycle{};

        lcd& screen;

        pixel_fifo background_fifo{};
        std::uint8_t window_line {};
        std::uint8_t window_fetcher_penalty{};
        bool window_active_in_scanline {};
        std::uint8_t pixels_drawn_in_scanline{};
        std::uint8_t pixels_to_discard{};
        pixel_fetcher pixel_fetcher;

        pixel_fifo sprite_fifo {};
        object_buffer sprite_buffer {};
        object_fetcher sprite_fetcher;

        ppu_interrupt_sources interrupt_sources{};
        bool stat_line{};
    };

    export std::uint8_t lcd_status(const pixel_processing_unit& ppu)
    {
        const auto [lyc_select, mode2, mode1, mode0] = ppu.interrupts();
        const bool lyc_equal_ly = ppu.lyc() == ppu.scanline();
        const std::uint8_t ppu_mode_reg = ppu.is_enabled()
            ? (std::to_underlying(ppu.mode()) & 0b11)
            : 0;

        return (lyc_select << 6)
            | (mode2 << 5)
            | (mode1 << 4)
            | (mode0 << 3)
            | (lyc_equal_ly << 2)
            | ppu_mode_reg;
    }

    export void set_lcd_status(const std::uint8_t status, pixel_processing_unit& ppu)
    {
        const ppu_interrupt_sources interrupts
        {
            utils::is_bit_set<6>(status),
            utils::is_bit_set<5>(status),
            utils::is_bit_set<4>(status),
            utils::is_bit_set<3>(status),
        };

        ppu.set_interrupts(interrupts);
    }

}
