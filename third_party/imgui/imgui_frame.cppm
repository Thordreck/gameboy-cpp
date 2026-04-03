module;
#include "imgui.h"
#include "upstream/imgui.h"

export module imgui:frame;

// TODO: improve this
namespace imgui
{
    export void new_frame()
    {
        ImGui::NewFrame();
    }

    export void render()
    {
        ImGui::Render();
    }

}