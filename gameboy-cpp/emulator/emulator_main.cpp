#include "profiling.hpp"

import std;
import emulator;
import utilities;

int main(int argc, char** argv)
{
    PROFILER_SESSION();

    using namespace emulator;

    joypad_source joypad {};
    gameboy emulator { joypad };
    graphical_interface ui { argc, argv };

    return ui.render(emulator);
}
