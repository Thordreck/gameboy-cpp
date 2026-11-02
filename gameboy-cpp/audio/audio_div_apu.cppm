
export module audio:div_apu;

import std;
import utilities;

namespace audio
{
    export class div_apu
    {
    public:
        [[nodiscard]] std::uint8_t value() const { return counter; }
        void reset() { }

        [[nodiscard]] bool tick(const std::uint16_t div)
        {
            const std::uint16_t falling_bits = prev_div & ~div;
            const bool should_advance = utils::is_bit_set<12>(falling_bits);

            prev_div = div;

            counter += should_advance;
            counter &= 0x7;

            return should_advance;
        }

    private:
        std::int8_t counter { -1 };
        std::uint16_t prev_div { 0 };
    };
}