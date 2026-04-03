module;
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

export module imgui:sdl3;
export import :common;
export import :assert;
export import sdl;
export import std;

namespace imgui
{
    export class sdl3_platform
    {
    public:
        [[nodiscard]] static result<sdl3_platform> with_renderer(sdl::window& window, sdl::renderer& renderer)
        {
            const assertion_scope error_scope {};

            if (ImGui_ImplSDL3_InitForSDLRenderer(
                sdl::internal::native::get_handle(window),
                sdl::internal::native::get_handle(renderer)))
            {
                return sdl3_platform {};
            }

            return std::unexpected { error_scope.error().value_or("Unexpected error") };
        }

        ~sdl3_platform()
        {
            if (valid)
            {
                ImGui_ImplSDL3_Shutdown();
                valid = false;
            }
        }

        sdl3_platform(const sdl3_platform&) = delete;
        sdl3_platform& operator=(const sdl3_platform&) = delete;

        sdl3_platform(sdl3_platform&& other) noexcept
            : sdl3_platform {}
        {
            valid = std::exchange(other.valid, false);
        }

        sdl3_platform& operator=(sdl3_platform&& other) noexcept
        {
            valid = std::exchange(other.valid, false);
            return *this;
        }

        void new_frame() const
        {
            if (valid)
            {
                ImGui_ImplSDL3_NewFrame();
            }
        }

        [[nodiscard]] bool process_event(const sdl::event& event) const
        {
            return valid ? ImGui_ImplSDL3_ProcessEvent(sdl::internal::native::get_handle(event)) : false;
        }

    private:
        sdl3_platform() = default;

        bool valid { true };
    };

    export class sdl3_renderer_backend
    {
    public:
        [[nodiscard]] static result<sdl3_renderer_backend> create(sdl::renderer& renderer)
        {
            const assertion_scope error_scope {};

            if (ImGui_ImplSDLRenderer3_Init(sdl::internal::native::get_handle(renderer)))
            {
                return sdl3_renderer_backend { &renderer };
            }

            return std::unexpected { error_scope.error().value_or("Unexpected error") };
        }

        ~sdl3_renderer_backend()
        {
            if (renderer != nullptr)
            {
                ImGui_ImplSDLRenderer3_Shutdown();
                renderer = nullptr;
            }
        }

        sdl3_renderer_backend(const sdl3_renderer_backend&) = delete;
        sdl3_renderer_backend& operator=(const sdl3_renderer_backend&) = delete;

        sdl3_renderer_backend(sdl3_renderer_backend&& other) noexcept
            : sdl3_renderer_backend { std::exchange(other.renderer, nullptr) }
        {}

        sdl3_renderer_backend& operator=(sdl3_renderer_backend&& other) noexcept
        {
            renderer = std::exchange(other.renderer, nullptr);
            return *this;
        }

        void new_frame() const
        {
            if (renderer != nullptr)
            {
                ImGui_ImplSDLRenderer3_NewFrame();
            }
        }

        void render() const
        {
            if (renderer != nullptr)
            {
                ImGui_ImplSDLRenderer3_RenderDrawData(
                    ImGui::GetDrawData(),
                    sdl::internal::native::get_handle(*renderer));
            }
        }

    private:
        explicit sdl3_renderer_backend(sdl::renderer* renderer)
            : renderer { renderer }
        {}

        sdl::renderer* renderer;
    };

}