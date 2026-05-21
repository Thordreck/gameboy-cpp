
export module audio:amplifier;
import :common;

namespace audio
{
    export template <AudioSample Sample, typename Gain>
    requires std::is_arithmetic_v<Gain>
    Sample apply_gain(const Sample input, const Gain factor)
    {
        return Sample { input * factor };
    }

}