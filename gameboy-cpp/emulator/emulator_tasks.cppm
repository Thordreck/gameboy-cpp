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

    export void engine_tick_thread(engine::Engine auto& engine, const std::stop_token& ct)
    {
        PROFILER_THREAD("Engine Thread");

        while (!ct.stop_requested())
        {
            PROFILER_SCOPE("Engine Frame");

            using namespace engine;
            utils::execute_for([&engine] { run_frame(engine); }
                               , frame_duration
                               , [](const auto& d) { utils::sleep_precise(d); });
        }
    };

    export void update_joypad_state(engine::Engine auto& engine, JoypadSource auto& source)
    {
        engine.update_joypad_state(source.read());
    }

    export void engine_joypad_thread(engine::Engine auto& engine, JoypadSource auto& source, const std::stop_token& ct)
    {
        PROFILER_THREAD("Joypad Thread");

        while (!ct.stop_requested())
        {
            PROFILER_SCOPE("Joypad Update");

            using namespace std::chrono_literals;
            utils::execute_for([&] { update_joypad_state(engine, source); }
                               , 16ms
                               , [](const auto& d) { std::this_thread::sleep_for(d); });
        }
    };

}
