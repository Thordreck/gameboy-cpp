
export module audio:div_apu;
import std;

namespace audio
{
    export class div_apu
    {
    public:
        [[nodiscard]] std::uint8_t value() const { return counter; }

        void tick()
        {
            if (++counter > 0b111)
            {
                counter = 0;
            }
        }

    private:
        std::uint8_t counter { 0 };

    };
}