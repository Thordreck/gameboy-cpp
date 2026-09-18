module;
#include <dsplib.h>

export module dsp:fft;

import std;
import :common;

namespace dsp
{
    export base_array<complex_t> fft(SpanLike<real_t> auto&& input)
    {
        dsplib::span_t span = dsplib::make_span(input);
        return dsplib::fft(span);
    }

    export base_array<complex_t> fft(const real_t* data, const size_t size, const size_t stride)
    {
        const auto slice = dsplib::slice_t<real_t>::make_slice(data, size, 0, size, stride);
        dsplib::arr_real array { slice };

        return fft(array);
    }

    export base_array<complex_t> fft(SliceLike<real_t> auto&& input)
    {
        return fft(input.data(), input.size(), input.stride());
    }

    export auto fft_shift(SpanLike<complex_t> auto&& input)
    {
        return dsplib::fftshift(input);
    }

    export auto fft_shift(SpanLike<real_t> auto&& input)
    {
        return dsplib::fftshift(input);
    }

}