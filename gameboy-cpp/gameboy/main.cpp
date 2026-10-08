#include "profiling.hpp"

import std;
import serial;
import joypad;
import emulator;

int main(int argc, char** argv)
{
    PROFILER_SESSION();

    using namespace serial;
    using namespace emulator;

    graphical_interface ui { argc, argv };

    joypad_source joypad {};
    audio_device audio {};
    dummy_link serial {};
    gameboy emulator { joypad, ui, audio, serial };

    return ui.render(emulator);
}
