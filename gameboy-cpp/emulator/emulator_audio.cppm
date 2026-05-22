export module emulator.audio;

#if defined(AUDIO_SDL)
export import :sdl;
#elif defined(AUDIO_QT)
export import :qt;
#else
#error "No audio backend configured"
#endif
