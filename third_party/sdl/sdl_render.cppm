module;
#include <SDL3/SDL_render.h>

export module sdl:render;
export import :common;
export import :window;

import std;
import :internal;

namespace sdl
{
    export class render_driver
    {
    public:
        [[nodiscard]] std::string_view name() const { return sdl_name; }

        static [[nodiscard]] std::vector<render_driver> get_available()
        {
            const int num_drivers = SDL_GetNumRenderDrivers();

            std::vector<render_driver> drivers;
            drivers.reserve(num_drivers);

            for (int i = 0; i < num_drivers; ++i)
            {
                drivers.push_back(render_driver{ SDL_GetRenderDriver(i) });
            }

            return drivers;
        }

    private:
        explicit render_driver(const std::string_view name)
            : sdl_name { name }
        {}

        std::string_view sdl_name;
        friend internal::wrapper;
    };

    export enum class renderer_vsync : std::int8_t
    {
        disabled = SDL_RENDERER_VSYNC_DISABLED,
        adaptive = SDL_RENDERER_VSYNC_ADAPTIVE,
    };

    export std::optional<render_driver> get_render_driver_by_name(const std::string_view name)
    {
        const auto available_drivers = render_driver::get_available();
        const auto found_it = std::ranges::find(available_drivers, name, &render_driver::name);

        return found_it != available_drivers.end() ? std::make_optional(*found_it) : std::nullopt;
    }

    export class renderer
    {
    public:
        static result<renderer> create(window& window, const std::optional<render_driver>& driver = std::nullopt)
        {
            const char* driver_name = driver
                .transform([] (const auto& value) { return value.name().data(); })
                .value_or(nullptr);

            if (SDL_Renderer* imp = SDL_CreateRenderer(internal::native::get_handle(window), driver_name); imp != nullptr)
            {
                return renderer(imp);
            }

            return std::unexpected(SDL_GetError());
        }

        [[nodiscard]] result<render_driver> driver() const
        {
            if (const char* name = SDL_GetRendererName(imp.get()); name != nullptr)
            {
                return internal::wrapper::create<render_driver>(name);
            }

            return std::unexpected(SDL_GetError());
        }

        [[nodiscard]] result<void> clear()
        {
            if (!SDL_RenderClear(imp.get()))
            {
                return std::unexpected(SDL_GetError());
            }

            return {};
        }

        [[nodiscard]] result<void> set_draw_color(const color& color)
        {
            if (!SDL_SetRenderDrawColor(imp.get(), color.r, color.g, color.b, color.a))
            {
                return std::unexpected(SDL_GetError());
            }

            return {};
        }

        [[nodiscard]] result<void> present()
        {
            if (!SDL_RenderPresent(imp.get()))
            {
                return std::unexpected(SDL_GetError());
            }

            return {};
        }

        [[nodiscard]] result<void> set_vsync(const renderer_vsync vsync)
        {
            return set_vsync(std::to_underlying(vsync));
        }

        [[nodiscard]] result<void> set_vsync(const int vsync)
        {
            if (!SDL_SetRenderVSync(imp.get(), vsync))
            {
                return std::unexpected(SDL_GetError());
            }

            return {};
        }

    private:
        explicit renderer(SDL_Renderer* imp) noexcept
            : imp { imp, SDL_DestroyRenderer }
        {}

        [[nodiscard]] auto native_handle() const { return imp.get(); }

        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> imp;
        friend internal::native;
    };

}
