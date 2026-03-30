
module;
#include "SDL3/SDL_surface.h"

export module sdl:surface;
export import :common;
export import std;

import :internal;

namespace sdl
{
    export class surface
    {
    public:
        [[nodiscard]] result<surface> create(
            const std::size_t width,
            const std::size_t height,
            const pixel_format format)
        {
            if (SDL_Surface* imp = SDL_CreateSurface(width, height, static_cast<SDL_PixelFormat>(format)); imp != nullptr)
            {
                return wrapper::create<surface>(imp);
            }

            return std::unexpected(SDL_GetError());
        }

    private:
        explicit surface(SDL_Surface* imp) noexcept
            : imp { imp, SDL_DestroySurface }
        {}

        [[nodiscard]] const SDL_Surface* native_handle() const noexcept { return imp.get(); }
        [[nodiscard]] SDL_Surface* native_handle() noexcept { return imp.get(); }

        std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> imp;

        friend native;
        friend wrapper;
    };

    export class surface_view
    {
        explicit surface_view(SDL_Surface* imp) noexcept
            : imp { imp }
        {}

        [[nodiscard]] auto native_handle() const { return imp; }

        SDL_Surface* imp;

        friend native;
        friend wrapper;

        // TODO: better way to expose this
    public:
        [[nodiscard]] void* pixels() { return imp->pixels; }
    };

}
