
export module audio:mixer;

import :common;

import std;
import utilities;

namespace audio
{
    export template<AudioSample Sample, size_t N>
    using mixed_sample = utils::constrained<typename Sample::underlying_type, N * Sample::min, N * Sample::max>;

    export template<AudioSample Sample, size_t N>
    using mixed_stereo_sample = stereo<mixed_sample<Sample, N>>;

    export template<AudioSample... Samples>
    requires utils::all_same<Samples...>
    auto mix(const stereo<Samples>&... samples)
    {
        using sample_t = std::tuple_element_t<0, std::tuple<Samples...>>;
        using raw_sample_t = sample_t::underlying_type;
        using mixed_t = mixed_sample<sample_t, sizeof...(Samples)>;
        using stereo_mixed_t = stereo<mixed_t>;

        return stereo_mixed_t
        {
            mixed_t{ (raw_sample_t{} + ... + samples.left.data()) },
            mixed_t{ (raw_sample_t{} + ... + samples.right.data()) }
        };
    }

}