export module emulator:ui;

#if defined(UI_SDL)
export import :ui.sdl;
#elif defined(UI_QT)
export import :ui.qt;
#else
#error "No UI backend configured"
#endif

