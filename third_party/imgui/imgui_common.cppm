module;
#include "imgui.h"

export module imgui:common;
export import std;

namespace imgui
{
    export template<typename T>
    using result = std::expected<T, std::string>;

    export void check_version()
    {
        IMGUI_CHECKVERSION();
    }

    export struct vec2
    {
        float x {};
        float y {};
    };

    export using texture_id = std::uintptr_t;

}