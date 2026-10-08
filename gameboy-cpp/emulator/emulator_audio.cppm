export module emulator:audio;

#if defined(AUDIO_SDL)
export import :audio.sdl;
#elif defined(AUDIO_QT)
export import :audio.qt;
#else
#error "No audio backend configured"
#endif
