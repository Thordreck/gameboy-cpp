
export module audio:normalization;

import utilities;
import :common;

namespace audio
{
    export template<AudioSample Sample, size_t N>
    using normalized_sample = utils::constrained<typename Sample::underlying_type, Sample::min / N, Sample::max / N>;

    export template<AudioSample Sample, size_t N>
    using normalized_stereo_sample = stereo<normalized_sample<Sample, N>>;

    export template<size_t N, AudioSample Sample>
    normalized_sample<Sample, N> normalize(const Sample sample)
    {
        return normalized_sample<Sample, N>{ sample.data() / N };
    }

    export template<size_t N, AudioSample Sample>
    normalized_stereo_sample<Sample, N> normalize(const stereo<Sample>& sample)
    {
        return { normalize<N>(sample.left), normalize<N>(sample.right) };
    }
}