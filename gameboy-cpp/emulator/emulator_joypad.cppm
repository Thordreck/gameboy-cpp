export module emulator:joypad;

#if defined(JOYPAD_SDL)
export import :joypad.sdl;
#elif defined(JOYPAD_QT)
export import :joypad.qt;
#else
#error "No joypad backend configured"
#endif

