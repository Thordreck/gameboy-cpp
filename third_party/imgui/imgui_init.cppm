module;
#include "imgui.h"

export module imgui:init;
export import :common;
export import :assert;
export import std;

namespace imgui
{
    export class context
    {
    public:
        static [[nodiscard]] result<context> create()
        {
            const assertion_scope error_scope {};

            if (ImGuiContext* imp = ImGui::CreateContext(); imp != nullptr)
            {
                return context(imp);
            }

            return std::unexpected { error_scope.error().value_or("Unexpected error") };
        }

    private:
        explicit context(ImGuiContext* imp)
            : imp { imp, ImGui::DestroyContext }
        {}

        std::unique_ptr<ImGuiContext, decltype(&ImGui::DestroyContext)> imp;
    };

}