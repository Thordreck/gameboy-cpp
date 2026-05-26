
export module audio:hpf;
import :common;

namespace audio
{
    // Adapted from https://gbdev.io/pandocs/Audio_details.html
    export template<AudioSample Sample>
    Sample apply_high_pass_filter(const Sample sample, float& capacitor)
    {
        using sample_t = Sample::underlying_type;

        const Sample filtered = sample - capacitor;
        capacitor = static_cast<sample_t>(sample) - static_cast<sample_t>(filtered) * 0.999958;

        return filtered;
    }

    export class high_pass_filter
    {
    public:
        template<AudioSample Sample>
        Sample apply(const Sample sample)
        {
            return apply_high_pass_filter(sample, capacitor);
        }

    private:
        float capacitor {};
    };

    export class stereo_high_pass_filter
    {
    public:
        template<AudioSample Sample>
        stereo<Sample> apply(const stereo<Sample> sample)
        {
            return { filters.left.apply(sample.left), filters.right.apply(sample.right) };
        }

    private:
        stereo<high_pass_filter> filters {};

    };

}