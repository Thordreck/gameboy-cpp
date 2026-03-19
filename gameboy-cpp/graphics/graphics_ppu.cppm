export module graphics:ppu;
export import memory;
export import :lcd;

import std;
import interrupts;
import utilities;

import :vram;
import :fifo;
import :common;
import :pixel_fetcher;

namespace
{
    bool is_window_present_in_scanline(const memory::memory_bus& memory)
    {
        using namespace graphics;
        return lcd_y(memory) >= wy(memory);
    }

    bool is_window_visible(const memory::memory_bus& memory, const std::uint8_t screen_x)
    {
        using namespace graphics;
        return is_window_present_in_scanline(memory) && screen_x >= static_cast<int>(wx(memory)) - 7;
    }

    bool is_window_first_line(const memory::memory_bus& memory)
    {
        using namespace graphics;
        return lcd_y(memory) == wy(memory);
    }

}

namespace graphics
{
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
        explicit pixel_processing_unit(lcd& screen, const_vram_t vram)
            : enabled{true}
            , screen(screen)
            , pixel_fetcher(background_fifo, vram)
        {}

        std::uint8_t [[nodiscard]] scanline() const { return current_scanline; }
        std::uint8_t [[nodiscard]] lyc() const { return scanline_compare; }
        ppu_mode [[nodiscard]] mode() const { return current_mode; }
        bool [[nodiscard]] is_enabled() const { return enabled; }
        ppu_interrupt_sources [[nodiscard]] interrupts() const { return interrupt_sources; }

        void tick()
        {
            if (!enabled)
            {
                return;
            }

            switch (current_mode)
            {
            case ppu_mode::h_blank:
                h_blank();
                break;
            case ppu_mode::v_blank:
                v_blank();
                break;
            case ppu_mode::oam_scan:
                scan_oam();
                break;
            case ppu_mode::drawing:
                draw();
                break;
            default: std::unreachable();
            }

            update_stat_line();
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
            update_stat_line();
        }

        void set_lyc(const std::uint8_t lyc)
        {
            scanline_compare = lyc;
            update_stat_line();
        }

        void connect(memory::memory_bus& bus)
        {
            pixel_fetcher.connect(bus);
            memory_bus = &bus;
        }

    private:
        void scan_oam()
        {
            // TODO: implement objects
            if (++scanline_cycle >= 80)
            {
                update_mode(ppu_mode::drawing);
            }
        }

        void draw()
        {
            // Draw start
            if (scanline_cycle++ == 80)
            {
                pixel_fetcher.reset();
                background_fifo.clear();
                pixels_drawn_in_scanline = 0;
                pixels_to_discard = scx(*memory_bus) % 8;
            }

            // Window start
            if (!window_active_in_scanline
                && is_window_enabled(*memory_bus)
                && is_window_visible(*memory_bus, pixels_drawn_in_scanline))
            {
                window_active_in_scanline = true;
                window_line = is_window_first_line(*memory_bus) ? 0 : window_line;

                background_fifo.clear();
                pixel_fetcher.reset();
                pixels_to_discard = 0;
                window_fetcher_penalty = wx(*memory_bus) > 0 ? 6 : 0;
            }

            if (window_fetcher_penalty > 0)
            {
                window_fetcher_penalty--;
                return;
            }

            pixel_fetcher.tick(window_active_in_scanline, window_line);

            if (const std::optional<pixel> pixel = background_fifo.try_pop(); pixel.has_value())
            {
                if (pixels_to_discard > 0)
                {
                    pixels_to_discard--;
                    return;
                }

                // TODO: make this configurable
                const color pixel_color = background_grayscale_color_palette[pixel.value().color_index];
                const coords_2d pixel_coords { pixels_drawn_in_scanline, current_scanline };

                screen.set_pixel(pixel_coords, pixel_color);
                pixels_drawn_in_scanline++;
            }

            if (pixels_drawn_in_scanline == 160)
            {
                update_mode(ppu_mode::h_blank);
            }
        }

        void h_blank()
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

                update_mode(next_mode);
            }
        }

        void v_blank()
        {
            if (++scanline_cycle >= 456)
            {
                scanline_cycle = 0;
                current_scanline++;

                if (current_scanline > 153)
                {
                    current_scanline = 0;
                    update_mode(ppu_mode::oam_scan);
                }
            }
        }

        void update_stat_line()
        {
            if (!enabled)
            {
                return;
            }

            const bool new_stat_line
                = (current_scanline == scanline_compare && interrupt_sources.lyc_select)
                || (current_mode == ppu_mode::h_blank && interrupt_sources.mode0)
                || (current_mode == ppu_mode::v_blank && interrupt_sources.mode1)
                || (current_mode == ppu_mode::oam_scan && interrupt_sources.mode2);

            const bool should_trigger_stat_interrupt
                = !stat_line && new_stat_line;

            if (should_trigger_stat_interrupt)
            {
                utils::assert_not_nullptr(memory_bus);
                interrupts::request<interrupts::lcd_interrupt>(*memory_bus);
            }

            stat_line = new_stat_line;
        }

        void update_mode(const ppu_mode new_mode)
        {
            current_mode = new_mode;

            // VBlank interrupt
            if (new_mode == ppu_mode::v_blank)
            {
                utils::assert_not_nullptr(memory_bus);
                interrupts::request<interrupts::vblank_interrupt>(*memory_bus);
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

        ppu_interrupt_sources interrupt_sources{};
        memory::memory_bus* memory_bus{nullptr};
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
