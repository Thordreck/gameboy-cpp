module;
#include <SDL3/SDL_video.h>

export module sdl:window;
export import :common;
export import std;

import :internal;

namespace sdl
{
    export enum class window_flags : std::uint64_t
    {
        fullscreen = SDL_WINDOW_FULLSCREEN,
        opengl = SDL_WINDOW_OPENGL,
        occluded = SDL_WINDOW_OCCLUDED,
        hidden = SDL_WINDOW_HIDDEN,
        borderless = SDL_WINDOW_BORDERLESS,
        resizable = SDL_WINDOW_RESIZABLE,
        minimized = SDL_WINDOW_MINIMIZED,
        maximized = SDL_WINDOW_MAXIMIZED,
        mouse_grabbed = SDL_WINDOW_MOUSE_GRABBED,
        input_focus = SDL_WINDOW_INPUT_FOCUS,
        mouse_focus = SDL_WINDOW_MOUSE_FOCUS,
        external = SDL_WINDOW_EXTERNAL,
        modal = SDL_WINDOW_MODAL,
        high_pixel_density = SDL_WINDOW_HIGH_PIXEL_DENSITY,
        mouse_capture = SDL_WINDOW_MOUSE_CAPTURE,
        mouse_relative_mode = SDL_WINDOW_MOUSE_RELATIVE_MODE,
        always_on_top = SDL_WINDOW_ALWAYS_ON_TOP,
        utility = SDL_WINDOW_UTILITY,
        tooltip = SDL_WINDOW_TOOLTIP,
        popup_menu = SDL_WINDOW_POPUP_MENU,
        keyboard_grabbed = SDL_WINDOW_KEYBOARD_GRABBED,
        fill_document = SDL_WINDOW_FILL_DOCUMENT,
        vulkan = SDL_WINDOW_VULKAN,
        metal = SDL_WINDOW_METAL,
        transparent = SDL_WINDOW_TRANSPARENT,
        not_focusable = SDL_WINDOW_NOT_FOCUSABLE,
    };

    export using window_id = std::uint32_t;

    export class window
    {
    public:
        static [[nodiscard]] result<window> create(
            const std::string_view title,
            const size_t width,
            const size_t height,
            const window_flags flags)
        {
            if (SDL_Window* imp = SDL_CreateWindow(title.data(), width, height, std::to_underlying(flags)); imp != nullptr )
            {
                return internal::wrapper::create<window>(imp);
            }

            return std::unexpected(SDL_GetError());
        }

        [[nodiscard]] window_id id() const { return SDL_GetWindowID(imp.get()); }

        template<typename Wrapper>
        requires internal::WrapperFor<Wrapper, SDL_Surface>
        [[nodiscard]] result<void> set_icon(Wrapper& icon)
        {
            if (!SDL_SetWindowIcon(imp.get(), internal::native::get_handle(icon)))
            {
                return std::unexpected(SDL_GetError());
            }

            return {};
        }

    private:
        explicit window(SDL_Window* imp)
            : imp(imp, SDL_DestroyWindow)
        {}

        [[nodiscard]] const SDL_Window* native_handle() const noexcept { return imp.get(); }
        [[nodiscard]] SDL_Window* native_handle() noexcept { return imp.get(); }

        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> imp;

        friend internal::native;
        friend internal::wrapper;
    };

}