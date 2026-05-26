
export module audio:amplifier;
import :common;

namespace audio
{
    export template <AudioSample Sample, typename Gain>
    requires std::is_arithmetic_v<Gain>
    Sample apply_gain(const Sample input, const Gain factor)
    {
        return Sample { static_cast<Sample::underlying_type>(input) * factor };
    }

    export template <AudioSample Sample, typename Gain>
    requires std::is_arithmetic_v<Gain>
    stereo<Sample> apply_gain(const stereo<Sample> input, const stereo<Gain> factor)
    {
        return { apply_gain(input.left, factor.left), apply_gain(input.right, factor.right) };
    }

}