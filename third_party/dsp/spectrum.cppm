export module dsp:spectrum;

import :fft;
import :math;
import :common;
import :window;

namespace dsp
{
    export auto spectrum(SpanLike<real_t> auto&& samples)
    {
        const auto hamming_window = hamming(samples.size());
        const auto windowed_samples = hamming_window * samples;
        const auto full_channel_fft = fft(windowed_samples) / windowed_samples.size();

        return abs(full_channel_fft.slice(0, full_channel_fft.size() / 2));
    }
}