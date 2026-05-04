module;
#include "profiling.hpp"

export module emulator.core:tasks;

import std;
import :common;

namespace emulator
{
    export constexpr std::uint32_t num_ticks_per_frame{70224};
    export constexpr std::chrono::nanoseconds tick_duration{238};
    export constexpr std::chrono::milliseconds frame_duration{
        std::chrono::duration_cast<std::chrono::milliseconds>(num_ticks_per_frame * tick_duration)
    };

    export template <Engine Imp>
    void run_frame(Imp& engine)
    {
        engine.tick(num_ticks_per_frame);
    }

    export template <Engine Imp>
    void engine_tick_thread(Imp& engine, const std::stop_token& ct)
    {
        PROFILER_THREAD("Engine Thread");

        while (!ct.stop_requested())
        {
            PROFILER_SCOPE("Engine Frame");

            utils::execute_for([&engine] { run_frame(engine); }
                               , frame_duration
                               , [](const auto& d) { utils::sleep_precise(d); });
        }
    };

    export template <Engine Imp, JoypadSource Source>
    void update_joypad_state(Imp& engine, Source& source)
    {
        engine.update_joypad_state(source.read());
    }

    export template <Engine Imp, JoypadSource Source>
    void engine_joypad_thread(Imp& engine, Source& source, const std::stop_token& ct)
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
