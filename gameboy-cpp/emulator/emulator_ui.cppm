export module emulator.ui;

#if defined(UI_SDL)
export import :sdl;
#else
#error "No UI backend configured"
#endif

