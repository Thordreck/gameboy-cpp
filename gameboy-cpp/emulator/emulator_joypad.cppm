export module emulator.joypad;

#if defined(JOYPAD_SDL)
export import :sdl;
#elif defined(JOYPAD_QT)
export import :qt;
#else
#error "No joypad backend configured"
#endif

