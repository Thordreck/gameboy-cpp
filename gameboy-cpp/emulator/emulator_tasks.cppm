module;
#include "profiling.hpp"

export module emulator:tasks;

import std;
import engine;
import joypad;
import utilities;

import :common;

namespace emulator
{
    export void run_frame(engine::Engine auto& engine)
    {
        using namespace engine;
        engine.tick(num_ticks_per_frame);
    }

    export void update_joypad_state(engine::Engine auto& engine, JoypadSource auto& source)
    {
        engine.update_joypad_state(source.read());
    }

    export void run_emulation(
        engine::Engine auto& engine,
        SynchronizationPoint auto& synchronizer,
        JoypadSource auto& joypad,
        const std::stop_token& ct)
    {
        PROFILER_THREAD("Engine Thread");

        while (!ct.stop_requested())
        {
            PROFILER_SCOPE("Engine Sync Frame");

            synchronizer.sync(ct);

            update_joypad_state(engine, joypad);
            engine.tick(synchronizer.ticks());
        }
    }

}
