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

    while (!ui.quit_app_requested())
    {
        PROFILER_SCOPE("UI Frame");

        using namespace std::chrono_literals;
        utils::execute_for(
            [&] { ui.render(emulator); }
            , 16ms
            , [](const auto& d) { std::this_thread::sleep_for(d); });
    }

    return 0;
}
