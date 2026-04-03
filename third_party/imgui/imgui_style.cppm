module;
#include "imgui.h"

export module imgui:style;

namespace imgui
{
    export void set_dark_style()
    {
        ImGui::StyleColorsDark();
    }

}