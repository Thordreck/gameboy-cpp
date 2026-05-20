
export module audio:hpf;
import :common;

namespace audio
{
    // Adapted from https://gbdev.io/pandocs/Audio_details.html
    export template<AudioSample Sample>
    Sample apply_high_pass_filter(const Sample sample, float& capacitor)
    {
        const Sample filtered = sample - capacitor;
        capacitor = sample - filtered * 0.999958;

        return filtered;
    }

}