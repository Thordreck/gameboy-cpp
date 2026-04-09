
export module emulator:utils;

import std;
import emulator.core;

namespace emulator
{
    export constexpr std::uint32_t num_ticks_per_frame { 70224 };
    export constexpr std::chrono::nanoseconds tick_duration { 238 };
    export constexpr std::chrono::milliseconds frame_duration { std::chrono::duration_cast<std::chrono::milliseconds>(num_ticks_per_frame * tick_duration) };

    export void run_frame(engine& engine)
    {
        for (std::size_t i = 0; i < num_ticks_per_frame; ++i)
        {
            engine.tick();
        }
    }

}
