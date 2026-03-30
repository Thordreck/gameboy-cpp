module;
#include <SDL3/SDL_rect.h>

export module sdl:rect;
export import std;

namespace sdl
{
    template<typename T>
    requires std::integral<T> || std::floating_point<T>
    struct base_rect
    {
        T x {};
        T y {};
        T w {};
        T h {};
    };

    export using rect = base_rect<int>;
    export using frect = base_rect<float>;

}