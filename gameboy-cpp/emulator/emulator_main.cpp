#include "profiling.hpp"

import std;
import emulator;
import utilities;

int main()
{
    PROFILER_SESSION();

    using namespace emulator;

    sdl_joypad_source joypad {};
    gameboy<engine, sdl_joypad_source> emulator { joypad };
    graphical_interface ui{ };

    while (true)
    {
        PROFILER_SCOPE("UI Frame");

        const auto event = utils::value_or_panic(sdl::wait_event());
        const bool should_quit = std::holds_alternative<sdl::quit_event>(event.parse());

        if (should_quit)
        {
            break;
        }

        ui.process_event(event);
        ui.render(emulator);
    }

    return 0;
}
