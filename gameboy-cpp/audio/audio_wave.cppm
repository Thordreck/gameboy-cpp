
export module audio:wave;

import std;
import :common;

namespace audio
{
    export class wave_channel
    {
    public:
        void trigger() {}
        [[nodiscard]] bool active() const { return false; }
        [[nodiscard]] digital_sample output() const { return {}; }

        void tick_length_timer() { }
        void tick(std::uint32_t num_ticks) {};
    };

}