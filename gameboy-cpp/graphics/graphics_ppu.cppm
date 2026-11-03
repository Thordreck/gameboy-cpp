module;
#include "profiling.hpp"

export module graphics:ppu;

import std;

import memory;
import utilities;
import interrupts;

import :lcd;
import :oam;
import :fifo;
import :common;
import :window;
import :objects;
import :background;
import :pixel_fetcher;
import :object_buffer;

namespace graphics
{
    [[nodiscard]] bool is_window_present_in_scanline(
        const std::uint8_t lcd_y,
        const std::uint8_t wy)
    {
        return lcd_y >= wy;
    }

    [[nodiscard]] bool is_window_visible(
        const std::uint8_t wx,
        const std::uint8_t wy,
        const std::uint8_t lcd_y,
        const std::uint8_t screen_x)
    {
        return is_window_present_in_scanline(lcd_y, wy) && screen_x >= static_cast<int>(wx) - 7;
    }

    [[nodiscard]] bool is_window_first_line(
        const std::uint8_t lcd_y,
        const std::uint8_t wy)
    {
        return lcd_y == wy;
    }

    [[nodiscard]] const pixel& mix(
        const pixel& background,
        const pixel& sprite,
        const bool bg_and_window_enabled,
        const bool objects_enabled)
    {
        // Sprites with color index zero are transparent.
        if (sprite.color_index == 0)
        {
            return background;
        }

        if (!objects_enabled)
        {
            return background;
        }

        if (!bg_and_window_enabled)
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

    [[nodiscard]] memory::memory_address_t get_palette_address(const pixel& pixel)
    {
        return pixel.palette_index
            .transform([] (const auto index) { return index == 1 ? obp1_address : obp0_address; })
            .value_or(bgp_address);
    }

    template<memory::ReadOnlyMemory Memory>
    [[nodiscard]] color get_pixel_color(const pixel& pixel, const Memory& memory, const lcd_color_palette_t palette)
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
        explicit pixel_processing_unit()
            : enabled{true}
        , pixel_fetcher(background_fifo)
        , sprite_fetcher(sprite_fifo)
        {}

        [[nodiscard]] std::uint8_t scanline() const
        {
            // If the cpu reads LY in the same m-cycle as h-blank ends, it reads the new value,
            return (current_mode == ppu_mode::h_blank && scanline_cycle >= 452)
                ? current_scanline + 1
                : current_scanline;
        }

        [[nodiscard]] std::uint16_t current_scanline_cycle() const { return scanline_cycle; }

        [[nodiscard]] std::uint8_t lyc() const { return scanline_compare; }
        [[nodiscard]] bool lyc_eq_ly() const
        {
            if (!is_enabled())
            {
                return latched_ly_lyc_bit;
            }

            const bool same_m_cycle_as_update = scanline_cycle == 454;
            const std::uint8_t ly_for_lyc = same_m_cycle_as_update ? 255 : scanline();

            return ly_for_lyc == lyc();
        }

        [[nodiscard]] ppu_mode mode() const { return initial_state_after_on ? ppu_mode::h_blank : current_mode; }
        [[nodiscard]] bool is_enabled() const { return enabled; }
        [[nodiscard]] ppu_interrupt_sources interrupts() const { return interrupt_sources; }

        void set_window_tilemap_area(const window_tilemap_address_mode mode) { window_tilemap_area = mode; }
        [[nodiscard]] window_tilemap_address_mode get_window_tilemap_area() const { return window_tilemap_area; }

        void set_window_enabled(const bool new_enabled) { window_enabled = new_enabled; }
        [[nodiscard]] bool get_window_enabled() const { return window_enabled; }

        void set_background_tile_data_area(const background_tile_data_area new_area) { bg_tile_data_area = new_area; }
        [[nodiscard]] background_tile_data_area get_background_tile_data_area() const { return bg_tile_data_area; }

        void set_background_tilemap_area(const background_tilemap_area new_area) { bg_tilemap_area = new_area; }
        [[nodiscard]] background_tilemap_area get_background_tilemap_area() const { return bg_tilemap_area; }

        void set_objects_size(const object_size new_size) { obj_size = new_size; }
        [[nodiscard]] object_size get_objects_size() const { return obj_size; }

        void set_objects_enabled(const bool new_enabled) { objects_enabled = new_enabled; }
        [[nodiscard]] bool get_objects_enabled() const { return objects_enabled; }

        void set_background_and_window_display_enabled(const bool new_enabled) { bg_and_window_display_enabled = new_enabled; }
        [[nodiscard]] bool get_background_and_window_display_enabled() const { return bg_and_window_display_enabled; }

        void set_scx(const std::uint8_t new_scx) { scx = new_scx; }
        [[nodiscard]] std::uint8_t get_scx() const { return scx; }

        void set_scy(const std::uint8_t new_scy) { scy = new_scy; }
        [[nodiscard]] std::uint8_t get_scy() const { return scy; }

        void set_wx(const std::uint8_t new_wx) { wx = new_wx; }
        [[nodiscard]] std::uint8_t get_wx() const { return wx; }

        void set_wy(const std::uint8_t new_wy) { wy = new_wy; }
        [[nodiscard]] std::uint8_t get_wy() const { return wy; }

        [[nodiscard]] bool active() const { return is_enabled(); }
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

        template<memory::Memory Memory, LCD Screen, interrupts::InterruptRequestController InterruptController>
        void tick(std::uint32_t num_ticks, Memory& memory, Screen& screen, InterruptController& interrupts)
        {
            PROFILER_SCOPE("PPU::tick()");

            if (!enabled) [[unlikely]] { return; }

            while (num_ticks > 0)
            {
                const std::uint32_t consumed_ticks = step(num_ticks, memory, screen, interrupts);
                num_ticks -= consumed_ticks;
            }
        }

        template<interrupts::InterruptRequestController InterruptController>
        void set_enabled(const bool enabled, InterruptController& interrupts)
        {
            const bool was_enabled = this->enabled;
            this->enabled = enabled;

            if (was_enabled && !enabled)
            {
                latched_ly_lyc_bit = scanline_compare == current_scanline;
                current_scanline = 0;

                // Note: the first time ly == 0 the ppu is delayed by 2 t-cycles
                scanline_cycle = 2;
                initial_state_after_on = true;
                current_mode = ppu_mode::oam_scan;

                window_line = 0;
                window_fetcher_penalty = 0;
                window_active_in_scanline = false;
                pixels_drawn_in_scanline = 0;
                pixels_to_discard = 0;
                background_fifo.clear();
                pixel_fetcher.reset();
            }
            if (!was_enabled && enabled)
            {
                update_stat_line(interrupts);
            }
        }

        template<interrupts::InterruptRequestController InterruptController>
        void set_interrupts(const ppu_interrupt_sources new_config, InterruptController& interrupts)
        {
            interrupt_sources = new_config;
            update_stat_line(interrupts);
        }

        template<interrupts::InterruptRequestController InterruptController>
        void set_lyc(const std::uint8_t lyc, InterruptController& interrupts)
        {
            scanline_compare = lyc;
            update_stat_line(interrupts);
        }

    private:
        template<memory::Memory Memory, LCD Screen, interrupts::InterruptRequestController InterruptController>
        std::uint32_t step(const std::uint32_t num_ticks, Memory& memory, Screen& screen, InterruptController& interrupts)
        {
            using enum ppu_mode;

            switch (current_mode)
            {
            case h_blank: return handle_h_blank(num_ticks,screen, interrupts);
            case v_blank: return handle_v_blank(num_ticks, interrupts);
            case oam_scan: return handle_scan_oam(num_ticks, memory, interrupts);
            case drawing: return handle_draw(num_ticks, memory, interrupts);
            default: std::unreachable();
            }
        }

        template<memory::Memory Memory, interrupts::InterruptRequestController InterruptController>
        std::uint32_t handle_scan_oam(const std::uint32_t num_ticks, Memory& memory, InterruptController& interrupts)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, tick_batch());

            for (std::uint32_t i = 0; i < ticks_consumed; i++)
            {
                if (scanline_cycle++ % 2 != 0) { continue; }

                const std::uint8_t object_index = static_cast<std::uint8_t>(scanline_cycle / 2);
                const object candidate = get_object(object_index, memory);
                const std::uint8_t objects_height = get_objects_height(obj_size);
                const bool is_visible = is_in_scanline(candidate, objects_height, current_scanline);

                if (is_visible && !sprite_buffer.is_full())
                {
                    sprite_buffer.push_back(candidate);
                }
            }

            if (scanline_cycle >= 80)
            {
                initial_state_after_on = false;

                current_mode = ppu_mode::drawing;
                pixel_fetcher.reset();
                background_fifo.clear();

                sprite_fetcher.reset();
                sprite_fifo.clear();

                pixels_drawn_in_scanline = 0;
                pixels_to_discard = scx % 8;

                update_stat_line(interrupts);
            }

            return ticks_consumed;
        }

        template<memory::Memory Memory, interrupts::InterruptRequestController InterruptController>
        std::uint32_t handle_draw(const std::uint32_t, Memory& memory, InterruptController& interrupts)
        {
            constexpr std::uint32_t ticks_consumed = 1;
            scanline_cycle += ticks_consumed;

            // Window start
            if (!window_active_in_scanline
                && window_enabled
                && is_window_visible(wx, wy, scanline(), pixels_drawn_in_scanline))
            {
                window_active_in_scanline = true;
                window_line = is_window_first_line(scanline(), wy) ? 0 : window_line;

                background_fifo.clear();
                pixel_fetcher.reset();
                pixels_to_discard = 0;
                window_fetcher_penalty = wx > 0 ? 6 : 0;
            }

            if (window_fetcher_penalty > 0)
            {
                window_fetcher_penalty--;
                return ticks_consumed;
            }

            // Sprite fetch
            // TODO: sprite fetch cancelling timing
            /*
            if (!objects_enabled)
            {
                sprite_fetcher.reset();
                sprite_fifo.clear();
            }
            */

            if (objects_enabled && !sprite_fetcher.is_fetching())
            {
                if (const std::optional next_object = pop_object_at_x(sprite_buffer, pixels_drawn_in_scanline);
                    next_object.has_value())
                {
                    sprite_fetcher.set_target(next_object.value(), window_active_in_scanline, scx);
                }
            }

            // Wait for background fifo to free the vram bus
            if (sprite_fetcher.is_fetching()
                && !(pixel_fetcher.dot() >= 6 || background_fifo.count() > 0))
            {
                tick_pixel_fetcher(memory);
                return ticks_consumed;
            }

            // Wait until sprite fetch completes
            if (sprite_fetcher.is_fetching())
            {
                tick_sprite_fetcher(memory);
                return ticks_consumed;
            }

            if (const std::optional<pixel> bg_pixel = background_fifo.try_pop(); bg_pixel.has_value())
            {
                if (pixels_to_discard > 0)
                {
                    tick_pixel_fetcher(memory);
                    pixels_to_discard--;

                    return ticks_consumed;
                }

                const std::optional sprite_pixel = sprite_fifo.try_pop();

                const pixel& mixed_pixel = sprite_pixel.has_value()
                    ? mix(bg_pixel.value(), sprite_pixel.value(), bg_and_window_display_enabled, objects_enabled)
                    : bg_pixel.value();

                // TODO: make palette configurable
                const color pixel_color = get_pixel_color(mixed_pixel, memory, grayscale_lcd_color_palette);

                // TODO: rethink this
                const std::size_t pixel_data_pos = pixels_drawn_in_scanline * num_color_channels;
                scanline_buffer[pixel_data_pos] = pixel_color.r;
                scanline_buffer[pixel_data_pos + 1] = pixel_color.g;
                scanline_buffer[pixel_data_pos + 2] = pixel_color.b;

                pixels_drawn_in_scanline++;
            }

            // Background/window fetch
            tick_pixel_fetcher(memory);

            if (pixels_drawn_in_scanline == 160)
            {
                current_mode = ppu_mode::h_blank;
                update_stat_line(interrupts);
            }

            return ticks_consumed;
        }

        template<LCD Screen, interrupts::InterruptRequestController InterruptController>
        std::uint32_t handle_h_blank(const std::uint32_t num_ticks, Screen& screen, InterruptController& interrupts)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, tick_batch());
            scanline_cycle += static_cast<std::uint16_t>(ticks_consumed);

            // End of scanline
            if (scanline_cycle >= 456)
            {
                scanline_cycle = 0;
                screen.write_scanline(current_scanline, scanline_buffer);

                current_scanline++;

                if (window_active_in_scanline)
                {
                    window_active_in_scanline = false;
                    window_line++;
                }

                if (current_scanline <= 143)
                {
                    sprite_buffer.clear();
                    current_mode = ppu_mode::oam_scan;
                }
                else
                {
                    current_mode = ppu_mode::v_blank;

                    screen.end_frame();
                    interrupts.request(interrupts::vblank_interrupt);
                }

                update_stat_line(interrupts);
            }

            return ticks_consumed;
        }

        template<interrupts::InterruptRequestController InterruptController>
        std::uint32_t handle_v_blank(const std::uint32_t num_ticks, InterruptController& interrupts)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, tick_batch());
            scanline_cycle += static_cast<std::uint16_t>(ticks_consumed);

            if (scanline_cycle >= 456)
            {
                scanline_cycle = 0;
                current_scanline++;

                if (current_scanline > 153)
                {
                    current_scanline = 0;
                    sprite_buffer.clear();
                    current_mode = ppu_mode::oam_scan;
                }

                update_stat_line(interrupts);
            }

            return ticks_consumed;
        }

        template<interrupts::InterruptRequestController InterruptController>
        void update_stat_line(InterruptController& interrupts)
        {
            if (!enabled) [[unlikely]] { return; }

            const bool new_stat_line
                = (scanline() == lyc() && interrupt_sources.lyc_select)
                || (current_mode == ppu_mode::h_blank && interrupt_sources.mode0)
                || (current_mode == ppu_mode::v_blank && interrupt_sources.mode1)
                || ((current_mode == ppu_mode::oam_scan || current_scanline == 144) && interrupt_sources.mode2);

            const bool should_trigger_stat_interrupt
                = !stat_line && new_stat_line;

            if (should_trigger_stat_interrupt)
            {
                interrupts.request(interrupts::lcd_interrupt);
            }

            stat_line = new_stat_line;
        }

        template<memory::Memory Memory>
        void tick_pixel_fetcher(Memory& memory)
        {
            pixel_fetcher.tick(
                window_active_in_scanline,
                window_line,
                window_tilemap_area,
                bg_tilemap_area,
                bg_tile_data_area,
                bg_and_window_display_enabled,
                scanline(),
                scx,
                scy,
                memory);
        }

        template<memory::Memory Memory>
        void tick_sprite_fetcher(Memory& memory)
        {
            sprite_fetcher.tick(scanline(), obj_size, memory);
        }

        bool enabled;
        ppu_mode current_mode{ppu_mode::oam_scan};
        std::uint8_t scanline_compare{};
        std::uint8_t current_scanline{};
        std::uint16_t scanline_cycle{};

        bg_fifo background_fifo{};
        std::uint8_t window_line {};
        std::uint8_t window_fetcher_penalty{};
        bool window_active_in_scanline {};
        std::uint8_t pixels_drawn_in_scanline{};
        std::uint8_t pixels_to_discard{};
        pixel_fetcher pixel_fetcher;

        sprite_fifo sprite_fifo {};
        object_buffer sprite_buffer {};
        object_fetcher sprite_fetcher;

        ppu_interrupt_sources interrupt_sources{};
        bool stat_line{};
        std::array<std::uint8_t, lcd_width * num_color_channels> scanline_buffer {};

        bool initial_state_after_on { false };
        bool latched_ly_lyc_bit { };

        bool window_enabled {};
        window_tilemap_address_mode window_tilemap_area {};

        background_tile_data_area bg_tile_data_area {};
        background_tilemap_area bg_tilemap_area {};

        bool objects_enabled {};
        object_size obj_size {};

        bool bg_and_window_display_enabled {};

        std::uint8_t scx {};
        std::uint8_t scy {};

        std::uint8_t wx {};
        std::uint8_t wy {};
    };

    export std::uint8_t lcd_status(const pixel_processing_unit& ppu)
    {
        const auto [lyc_select, mode2, mode1, mode0] = ppu.interrupts();
        const bool lyc_equal_ly = ppu.lyc_eq_ly();
        const std::uint8_t ppu_mode_reg = ppu.is_enabled()
            ? (std::to_underlying(ppu.mode()) & 0b11)
            : 0;

        return (lyc_select << 6)
            | (mode2 << 5)
            | (mode1 << 4)
            | (mode0 << 3)
            | (lyc_equal_ly << 2)
            | ppu_mode_reg
            | 0x80;
    }

    export template <interrupts::InterruptRequestController InterruptController>
    void set_lcd_status(const std::uint8_t status, pixel_processing_unit& ppu, InterruptController& interrupts)
    {
        const ppu_interrupt_sources sources
        {
            utils::is_bit_set<6>(status),
            utils::is_bit_set<5>(status),
            utils::is_bit_set<4>(status),
            utils::is_bit_set<3>(status),
        };

        ppu.set_interrupts(sources, interrupts);
    }

}
