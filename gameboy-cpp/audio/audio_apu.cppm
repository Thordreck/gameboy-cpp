
export module audio:apu;

import std;

namespace audio
{
    export class audio_processing_unit
    {
    public:
        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool value) { enabled = value; }

        [[nodiscard]] bool active() const { return is_enabled(); }
        [[nodiscard]] std::uint32_t tick_batch() const
        {
            if (!active()) [[unlikely]] { return std::numeric_limits<std::uint32_t>::max(); }

            // TODO: implement properly
            return 1;
        }

        void tick(const std::uint32_t ticks)
        {
        }

    private:
        bool enabled { true };
    };

}