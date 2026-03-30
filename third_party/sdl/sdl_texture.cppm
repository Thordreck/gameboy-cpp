module;
#include "SDL3/SDL_render.h"

export module sdl:texture;
export import :common;
export import :render;
export import :surface;
export import :rect;
export import std;

import :internal;

namespace sdl
{
    export enum class texture_access
    {
        changeless = SDL_TEXTUREACCESS_STATIC,
        streaming = SDL_TEXTUREACCESS_STREAMING,
        target = SDL_TEXTUREACCESS_TARGET
    };

    export class texture
    {
    public:
        static [[nodiscard]] result<texture> create(
            renderer& renderer,
            const pixel_format format,
            const texture_access access,
            const std::size_t width,
            const std::size_t height)
        {
            if (SDL_Texture* imp = SDL_CreateTexture(
                native::get_handle(renderer),
                static_cast<SDL_PixelFormat>(format),
                static_cast<SDL_TextureAccess>(access),
                width, height); imp != nullptr)
            {
                return texture { imp };
            }

            return std::unexpected{ SDL_GetError() };
        }

    private:
        explicit texture(SDL_Texture* imp) noexcept
            : imp { imp, SDL_DestroyTexture }
        {}

        [[nodiscard]] auto native_handle() const { return imp.get(); }

        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> imp;
        friend native;
    };

    export result<void> render(
        renderer& renderer,
        texture& texture,
        const std::optional<frect>& source_rect = std::nullopt,
        const std::optional<frect>& destination_rect = std::nullopt)
    {
        const SDL_FRect* src_rect = source_rect
            .transform([] (const auto& rect) { return reinterpret_cast<const SDL_FRect*>(&rect); })
            .value_or(nullptr);

        const SDL_FRect* dst_rect = destination_rect
            .transform([] (const auto& rect) { return reinterpret_cast<const SDL_FRect*>(&rect); })
            .value_or(nullptr);

        if (!SDL_RenderTexture(native::get_handle(renderer), native::get_handle(texture), src_rect, dst_rect))
        {
            return std::unexpected{ SDL_GetError() };
        }

        return {};
    }

    export class texture_lock
    {
    public:
        ~texture_lock() noexcept
        {
            if (texture != nullptr)
            {
                SDL_UnlockTexture(texture);
            }
        }

        texture_lock(const texture_lock&) = delete;
        texture_lock& operator=(const texture_lock&) = delete;

        texture_lock(texture_lock&& other) noexcept
            : texture { std::exchange(other.texture, nullptr ) }
        {}

        texture_lock& operator=(texture_lock&& other) noexcept
        {
            texture = std::exchange(other.texture, nullptr);
            return *this;
        }

    private:
        explicit texture_lock(texture& texture) noexcept
            : texture { native::get_handle(texture) }
        {}

        SDL_Texture* texture;
        friend wrapper;
    };

    export result<std::tuple<texture_lock, surface_view>> lock_to_surface(
        texture& texture,
        const std::optional<rect>& rect = std::nullopt)
    {
        SDL_Surface* sdl_surface = nullptr;
        SDL_Texture* sdl_texture = native::get_handle(texture);

        const SDL_Rect* sdl_rect = rect
            .transform([] (const auto& r) { return reinterpret_cast<const SDL_Rect*>(&r); })
            .value_or(nullptr);

        if (!SDL_LockTextureToSurface(sdl_texture, sdl_rect, &sdl_surface))
        {
            return std::unexpected{ SDL_GetError() };
        }

        return std::make_tuple(
            wrapper::create<texture_lock>(texture),
            wrapper::create<surface_view>(sdl_surface));
    }

}
