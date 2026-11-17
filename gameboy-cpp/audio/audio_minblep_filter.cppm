
export module audio:minblep_filter;

import std;

import :minblep;

namespace audio
{
    export template <std::floating_point SampleType, size_t ZeroCrossings, size_t OverSampling>
    class minblep_filter
    {
    public:
        minblep_filter()
            : min_blep_signal { min_blep<SampleType, ZeroCrossings, OverSampling>() }
        {}

        // TODO: implement
        [[nodiscard]] SampleType apply(const SampleType input)
        {
            //std::println("Minblep filter input {}", input);
            return input;
        }

    private:
        min_blep_t<SampleType, ZeroCrossings, OverSampling> min_blep_signal;

    };

    export using default_minblep_filter = minblep_filter<float, 32, 64>;

}