module;
#include <dsplib.h>

export module dsp:spectrum;
import :common;

namespace dsp
{
    export auto spectrum(SpanLike<const real_t> auto const& samples)
    {
        const auto hamming_window = dsplib::window::hamming(samples.size());
        const auto windowed_samples = hamming_window * make_span(samples);
        const auto full_channel_fft = dsplib::fft(windowed_samples) / windowed_samples.size();

        return to_vector(dsplib::abs(full_channel_fft.slice(0, full_channel_fft.size() / 2)));
    }

    export auto nfft(SpanLike<const real_t> auto const& samples)
    {
        const auto hamming_window = dsplib::window::hamming(samples.size());
        const auto windowed_samples = hamming_window * make_span(samples);

        return windowed_samples.size();
    }

}