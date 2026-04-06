
export module audio:channels;
import :common;

namespace audio
{
    export class pulse_channel
    {
    public:
        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool value) { enabled = value; }

        [[nodiscard]]
    private:
        bool enabled { true };
    };

}