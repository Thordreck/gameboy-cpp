export module emulator.ui;

#if defined(UI_SDL)
export import :sdl;
#elif defined(UI_QT)
export import :qt;
#else
#error "No UI backend configured"
#endif

