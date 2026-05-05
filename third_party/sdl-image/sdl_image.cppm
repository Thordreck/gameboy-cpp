module;
#include "SDL3_image/SDL_image.h"

export module sdl_image;
export import std;
export import sdl;

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

}
