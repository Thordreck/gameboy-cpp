module;
#include <dsplib.h>

export module dsp:fft;

import std;
import :common;

namespace dsp
{
    export std::vector<complex_t> fft(SpanLike<real_t> auto&& input)
    {
        dsplib::span_t span = dsplib::make_span(input);
        const dsplib::arr_cmplx result = dsplib::fft(span);

        std::vector<complex_t> output(result.size());
        std::ranges::copy(result.begin(), result.end(), output.begin());

        return output;
    }

    export std::vector<complex_t> fft(const real_t* data, size_t size, size_t stride)
    {
        const auto slice = dsplib::slice_t<real_t>::make_slice(data, size, 0, size, stride);
        dsplib::arr_real array { slice };

        return fft(array);
    }

    export std::vector<complex_t> fft(SliceLike<real_t> auto&& input)
    {
        return fft(input.data(), input.size(), input.stride());
    }

}