module;
#include <SDL3/SDL_events.h>

export module sdl:events;
export import :window;
export import std;

import :internal;

namespace sdl
{
    export using event_timestamp_t = std::chrono::nanoseconds;

    export class quit_event
    {
    public:
        [[nodiscard]] event_timestamp_t timestamp() const noexcept { return time; }

    private:
        explicit quit_event(const SDL_QuitEvent& event) noexcept
            : time { event.timestamp }
        {}

        event_timestamp_t time;
        friend wrapper;
    };

    export class window_shown_event
    {
    public:
        [[nodiscard]] event_timestamp_t timestamp() const noexcept { return time; }

    private:
        explicit window_shown_event(const SDL_WindowEvent& event) noexcept
            : time { event.timestamp }
        {}

        event_timestamp_t time{};
        friend wrapper;
    };

    export using event_t = std::variant<quit_event, window_shown_event>;

    export std::optional<event_t> poll_event()
    {
        SDL_Event event {};

        if (!SDL_PollEvent(&event))
        {
            return std::nullopt;
        }

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            return wrapper::create<quit_event>(event.quit);
        case SDL_EVENT_WINDOW_SHOWN:
            return wrapper::create<window_shown_event>(event.window);
        default:
            return std::nullopt;
        }
    }

}