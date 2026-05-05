module;
#include "imgui.h"

export module imgui:viewport;

import std;
import :common;

namespace imgui
{
    export class viewport
    {
    public:
        static result<viewport> get_main()
        {
            ImGuiViewport* impl = ImGui::GetMainViewport();

            if (impl == nullptr)
            {
                return std::unexpected("No viewport found");
            }

            return viewport { impl };
        }

        [[nodiscard]] unsigned int id() const { return impl->ID; }
        [[nodiscard]] vec2 position() const { return { impl->Pos.x, impl->Pos.y }; }
        [[nodiscard]] vec2 size() const { return { impl->Size.x, impl->Size.y }; }
        [[nodiscard]] vec2 center() const
        {
            const auto impl_center = impl->GetCenter();
            return { impl_center.x, impl_center.y };
        }

        [[nodiscard]] vec2 work_area_position() const { return { impl->WorkPos.x, impl->WorkPos.y }; }
        [[nodiscard]] vec2 work_area_size() const { return { impl->WorkSize.x, impl->WorkSize.y }; }
        [[nodiscard]] vec2 work_area_center() const
        {
            const auto impl_center = impl->GetWorkCenter();
            return { impl_center.x, impl_center.y };
        }

    private:
        explicit viewport(ImGuiViewport* imgui_viewport)
            : impl { imgui_viewport }
        {}

        ImGuiViewport* impl;
    };

}
