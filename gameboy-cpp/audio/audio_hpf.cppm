
export module audio:hpf;
import :common;

namespace audio
{
    [[nodiscard]] float compute_high_pass_filter_charge_factor(const std::uint32_t sample_rate)
    {
        constexpr float base_factor { 0.999958f };
        return std::pow(base_factor, 4194304 / static_cast<float>(sample_rate));
    }

    // Adapted from https://gbdev.io/pandocs/Audio_details.html
    export template<AudioSample Sample>
    [[nodiscard]] Sample apply_high_pass_filter(const Sample sample, float& capacitor, const float charge_factor)
    {
        using sample_t = Sample::underlying_type;

        const Sample filtered = sample - capacitor;
        capacitor = static_cast<sample_t>(sample) - static_cast<sample_t>(filtered) * charge_factor;

        return filtered;
    }

    export class high_pass_filter
    {
    public:
        template<AudioSample Sample>
        [[nodiscard]] Sample apply(const Sample sample, const float charge_factor)
        {
            return apply_high_pass_filter(sample, capacitor, charge_factor);
        }

    private:
        float capacitor {};
    };

    export class stereo_high_pass_filter
    {
    public:
        template<AudioSample Sample>
        [[nodiscard]] stereo<Sample> apply(const stereo<Sample> sample, const float charge_factor)
        {
            return { filters.left.apply(sample.left, charge_factor), filters.right.apply(sample.right, charge_factor) };
        }

    private:
        stereo<high_pass_filter> filters {};
    };

}