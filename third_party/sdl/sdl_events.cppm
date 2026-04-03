module;
#include <SDL3/SDL_events.h>

export module sdl:events;
export import :window;
export import :internal;
export import std;

namespace sdl
{
    export using event_timestamp_t = std::chrono::nanoseconds;

    export class common_event
    {
    public:
        [[nodiscard]] event_timestamp_t timestamp() const noexcept { return time; }

    private:
        explicit common_event(const SDL_CommonEvent& event) noexcept
            : time { event.timestamp }
        {}

        event_timestamp_t time;
        friend internal::wrapper;
    };

    export class quit_event
    {
    public:
        [[nodiscard]] event_timestamp_t timestamp() const noexcept { return time; }

    private:
        explicit quit_event(const SDL_QuitEvent& event) noexcept
            : time { event.timestamp }
        {}

        event_timestamp_t time;
        friend internal::wrapper;
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
        friend internal::wrapper;
    };

    export using event_variants_t = std::variant<common_event, quit_event, window_shown_event>;

    event_variants_t parse_event(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            return internal::wrapper::create<quit_event>(event.quit);
        case SDL_EVENT_WINDOW_SHOWN:
            return internal::wrapper::create<window_shown_event>(event.window);
        default:
            return internal::wrapper::create<common_event>(event.common);
        }
    }

    export class event
    {
    public:
        [[nodiscard]] event_variants_t parse() const
        {
            return parse_event(sdl_event);
        }

    private:
        explicit event(const SDL_Event& event)
            : sdl_event { event }
        {}

        [[nodiscard]] auto native_handle() const { return &sdl_event; }

        SDL_Event sdl_event;

        friend internal::wrapper;
        friend internal::native;
    };

    export std::optional<event> poll_event()
    {
        SDL_Event sdl_event {};

        if (!SDL_PollEvent(&sdl_event))
        {
            return std::nullopt;
        }

        return internal::wrapper::create<event>(sdl_event);
    }

}