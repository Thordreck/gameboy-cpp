
export module tests.core:engine;

import std;
import engine;

namespace tests
{
    export void tick_frames(engine::Engine auto& engine, const std::size_t num_frames)
    {
        for (size_t i = 0; i < num_frames; i++)
        {
            constexpr std::uint32_t num_ticks_per_frame { 70224 };
            engine.tick(num_ticks_per_frame);
        }
    }

}