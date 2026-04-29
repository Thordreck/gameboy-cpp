#include "profiling.hpp"

import std;
import emulator;
import utilities;

int main()
{
    PROFILER_SESSION();

    using namespace emulator;

    sdl_joypad_source joypad {};
    gameboy emulator { joypad };
    graphical_interface ui{ };
    ui.render(emulator);

    return 0;
}
