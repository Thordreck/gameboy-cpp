module;
#include <dsplib.h>

export module dsp:fft;

import std;
import :common;

namespace dsp
{
    export auto fft(SpanLike<const real_t> auto const& input)
    {
        return to_vector(dsplib::fft(make_span(input)));
    }

    export auto fft(SpanLike<const complex_t> auto const& input)
    {
        return to_vector(dsplib::fft(make_span(input)));
    }

    export template<RealOrComplex T>
    auto fft(const T* data, const size_t size)
    {
        return to_vector(dsplib::fft(make_span(data, size)));
    }

    export auto fft(SliceLike<const real_t> auto const& input)
    {
        return to_vector(fft(make_array(input)));
    }

    export auto fft(SliceLike<const complex_t> auto const& input)
    {
        return to_vector(fft(make_array(input)));
    }

    export template<RealOrComplex T>
    auto fft(const T* data, const size_t size, const size_t stride)
    {
        return to_vector(fft(make_array(data, size, stride)));
    }

}