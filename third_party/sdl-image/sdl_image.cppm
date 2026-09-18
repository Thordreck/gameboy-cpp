module;
#include "SDL3_image/SDL_image.h"

export module sdl_image;
import std;
import sdl;

namespace sdl
{
    export [[nodiscard]] result<surface> load_image(const std::filesystem::path& path)
    {
        if (SDL_Surface* imp = IMG_Load(path.string().data()); imp != nullptr)
        {
            return internal::wrapper::create<surface>(imp);
        }

        return std::unexpected(SDL_GetError());
    }

    export [[nodiscard]] result<surface> load_image(iostream& stream)
    {
        auto* stream_imp = internal::native::get_handle(stream);

        if (SDL_Surface* imp = IMG_Load_IO(stream_imp, false); imp != nullptr)
        {
            return internal::wrapper::create<surface>(imp);
        }

        return std::unexpected(SDL_GetError());
    }

    export [[nodiscard]] result<surface> load_sized_svg(iostream& stream, const int width, const int height)
    {
        auto* stream_imp = internal::native::get_handle(stream);

        if (SDL_Surface* imp = IMG_LoadSizedSVG_IO(stream_imp, width, height); imp != nullptr)
        {
            return internal::wrapper::create<surface>(imp);
        }

        return std::unexpected(SDL_GetError());
    }

    export [[nodiscard]] outcome save_image(surface& surface, const std::filesystem::path& path)
    {
        if (IMG_Save(internal::native::get_handle(surface), path.string().c_str()))
        {
            return {};
        }

        return std::unexpected(SDL_GetError());
    }

}
