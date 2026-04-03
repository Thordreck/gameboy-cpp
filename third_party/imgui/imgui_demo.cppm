module;
#include "imgui.h"

export module imgui:demo;
export import :common;
export import :assert;

namespace imgui
{
    export void show_demo_window()
    {
        //const assertion_scope error_scope {};
        ImGui::ShowDemoWindow();

        /*
        if (const auto error = error_scope.error(); error.has_value())
        {
            return std::unexpected { error.value() };
        }

        return{};
        */
    }

}