
export module audio:minblep;
import std;

import :utils;
import :window;
import :signals;

namespace audio
{
    // Adapted from: https://www.experimentalscene.com/articles/minbleps.php
    export template <std::floating_point SampleType, size_t ZeroCrossings, size_t OverSampling>
    requires (ZeroCrossings > 0) && (OverSampling > 0)
    constexpr [[nodiscard]] auto min_blep_table() noexcept
    {
        // TODO: missing + 1?
        constexpr size_t table_length { ZeroCrossings * 2 * OverSampling  };

        // Sinc
        const auto sincs = make_array<table_length>([] (const size_t i) constexpr
        {
            const SampleType x = math::fma(
                static_cast<SampleType>(2 * ZeroCrossings),
                i / static_cast<SampleType>(table_length - 1),
                -static_cast<SampleType>(ZeroCrossings));

            return sinc(x);
        });

        // Windowing
        const auto window = blackman_window<SampleType, table_length>();
        const auto band_limited_sincs = make_array<table_length>([&](const size_t i) constexpr { return sincs[i] * window[i]; });

        // Minimum phase
        const auto real_cepstrum = rceps(band_limited_sincs);
        const auto minimum_phase = min_phase(real_cepstrum);

        // Integration
        SampleType accumulator {};
        const auto integration = make_array<table_length>([&] (const size_t i) constexpr
        {
            accumulator += minimum_phase[i];
            return accumulator;
        });

        // Normalization
        const SampleType normalization_factor {  1 / integration[table_length - 1] };
        return make_array<table_length>([&] (const size_t i) constexpr
        {
            return integration[i] * normalization_factor;
        });
    }

}