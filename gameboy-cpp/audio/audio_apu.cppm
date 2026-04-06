
export module audio:apu;

namespace audio
{
    export class audio_processing_unit
    {
    public:
        [[nodiscard]] bool is_enabled() const { return enabled; }
        void set_enabled(const bool value) { enabled = value; }

        void tick()
        {
        }

    private:
        bool enabled { true };
    };

}