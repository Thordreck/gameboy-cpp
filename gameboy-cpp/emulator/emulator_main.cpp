#include "profiling.hpp"

import std;
import serial;
import emulator;
import utilities;

int main(int argc, char** argv)
{
    PROFILER_SESSION();

    using namespace emulator;

    joypad_source joypad {};
    audio_device audio {};
    serial::dummy_link serial {};
    gameboy emulator { joypad, audio, serial };
    graphical_interface ui { argc, argv };

    return ui.render(emulator);
}
